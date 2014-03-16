#include "snygg_instance.hpp"

#include <fileutil.hpp>
#include <lua_board_provider.hpp>
#include <board.hpp>
#include <rect.hpp>
#include <complex_polygon.hpp>
#include <load_jpeg.hpp>
#include "urlloader_file_loader.hpp"
#include "iurlstream.hpp"
#include "ologstream.hpp"

#include <ppapi/cpp/graphics_3d.h>
#include <GLES2/gl2.h>
#include <ppapi/lib/gl/gles2/gl2ext_ppapi.h>
#include <ppapi/cpp/completion_callback.h>
#include <ppapi/cpp/message_loop.h>
#include <ppapi/cpp/var.h>
#include <ppapi/cpp/input_event.h>

#include <algorithm>
#include <matrix3d.hpp>
#include <keyboard_handler.hpp>
#include <keycodes.hpp>

#include <food_generator.hpp>
#include "recording_skin.hpp"

#include "buffering_skin.hpp"


namespace attrib {
	enum {
		vertex = 0,
		circle_coord,
		across,
		along,
		b
	};
}


std::pair<std::shared_ptr<board_provider>, std::shared_ptr<board>>
load_board(pp::InstanceHandle instanceHandle, const std::string& boardname) {
	urlloader_file_loader loader(instanceHandle);

	auto bp = std::shared_ptr<lua_board_provider>(new lua_board_provider(loader, boardname.length() ? boardname : "wide_screen"));
	auto b = std::shared_ptr<board>(new board(*bp));
	return std::make_pair(bp, b);
}


template <typename F>
void dispatchFunc(void* userdata, int32_t result) {
	std::unique_ptr<F> f{static_cast<F*>(userdata)};
	(*f)(result);
}

template <typename F>
pp::CompletionCallback makeCompletionCallback(F func) {
	return pp::CompletionCallback(&dispatchFunc<F>, new F(std::move(func)));
}


template <typename F1, typename F2>
std::thread async(F1&& asyncOperation, F2&& callback) {
	auto messageLoop = pp::MessageLoop::GetCurrent();
	return std::thread([=]() mutable {
		std::shared_ptr<decltype(asyncOperation())> result;
		result.reset(new decltype(asyncOperation()) (asyncOperation()));
		auto postResult = messageLoop.PostWork(
			makeCompletionCallback(
				// FIXME Undesirable shared_ptr. Could we move the result object directly?
				[=](int32_t) mutable { callback(std::move(*result)); }
			)
		);
		if (postResult != PP_OK) std::terminate();
	});
}


snygg_instance::snygg_instance(PP_Instance instance) :
	Instance(instance),
	Graphics3DClient(this),
	lout(*this, PP_LOGLEVEL_LOG)
{
}


snygg_instance::~snygg_instance() {
	load_board_thread.join();
}

void snygg_instance::add_item(std::unique_ptr<item>&& i) {
	new_items.emplace_back(std::move(i));
}

void snygg_instance::add_renderable(std::unique_ptr<renderable>&& r) {
	renderables.emplace_back(std::move(r));
}

pp::Graphics3D initGL(pp::Instance instance, int32_t width, int32_t height) {
	const int32_t attrib_list[] = {
		PP_GRAPHICS3DATTRIB_ALPHA_SIZE, 1,
		PP_GRAPHICS3DATTRIB_DEPTH_SIZE, 0,
		PP_GRAPHICS3DATTRIB_WIDTH, width,
		PP_GRAPHICS3DATTRIB_HEIGHT, height,
		PP_GRAPHICS3DATTRIB_NONE
	};

	pp::Graphics3D context = pp::Graphics3D(pp::InstanceHandle(&instance), attrib_list);

	if (!instance.BindGraphics(context)) throw std::runtime_error("Unable to bind 3d context!");

	glSetCurrentContextPPAPI(context.pp_resource());

	GLboolean shaderCompilerSupported;
	glGetBooleanv(GL_SHADER_COMPILER, &shaderCompilerSupported);
	if (shaderCompilerSupported == GL_FALSE) {
		instance.LogToConsole(PP_LOGLEVEL_ERROR, "GLSL shader compiler not supported. Bailing out.");
		std::terminate();
	}

	return context;
}

void renderLoopTrampoline(void* userdata, int32_t) {
	auto fp = static_cast<std::weak_ptr<std::function<void(void*)>>*>(userdata);

	auto f = fp->lock();
	if (f) (*f)(userdata);
	else delete fp;
}

void snygg_instance::check_gl_error() {
	auto status = glGetError();
	if (status != GL_NO_ERROR) {
		LogToConsole(PP_LOGLEVEL_ERROR, pp::Var((int32_t)status));
	}
}

void snygg_instance::tick_10ms() {
	for (auto& item : new_items) {
		items.emplace_back(std::move(item));
	}
	new_items.clear();

	for (auto& item : items) {
		if (!item->is_dead()) item->move();
	}

	std::vector<player*> dead_players;
	for (auto& player : players) {
		for (auto& item : items) {
			if (!item->is_dead() && player->crashes_with(*item)) item->hit_by(*player);
		}
		if (player->crashes_with(*bp)) {
			dead_players.push_back(&*player);
		}
	}

	for (auto& dead_player : dead_players) dead_player->die();

	auto new_end = std::remove_if(items.begin(), items.end(), [](const std::unique_ptr<item>& i) { return i->is_dead(); });
	items.erase(new_end, items.end());
}

void snygg_instance::simulate_until(PP_TimeTicks nowTimeTicks) {
	int now = (nowTimeTicks - startTime) * 100;

	if (now > ticks + 200) ticks = now;

	while (ticks < now) {
		tick_10ms();
		++ticks;
	}
}

void snygg_instance::render(void* userdata) {
	simulate_until(pp::Module::Get()->core()->GetTimeTicks());


	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	skin->enter_state(skin::floor_state);
	floor.render(attrib::vertex);


	skin->enter_state(skin::other_state);
	for (auto& renderable : renderables) renderable->render(*skin);
	for (auto& item : items) item->render(*skin);

	skin->enter_state(skin::board_state);

	walls.render(*skin);


	context.SwapBuffers(pp::CompletionCallback(&renderLoopTrampoline, userdata));
}

void snygg_instance::maybe_ready() {
	if (!bp) return;
	if (!resources_loaded) return;
	if (!images_loaded) return;
	if (context.is_null()) return;


	skin.reset(new buffering_skin(resources, images, lout));
	skin->set_transformation(reshaper.get_transformation());
	skin->set_pixels_per_unit(reshaper.get_pixels_per_unit());

	floor = std::move(renderable_complex_polygon(bp->floor_polygon()));

	update_walls();

	check_gl_error();


	bp->get_starting_position();
	players.emplace_back(new player(kbd, *this, *bp, game::KEY_LEFT, game::KEY_RIGHT, game::KEY_SPACE));


	fg.reset(new food_generator(*this, *bp));
	fg->generate();


	PostMessage(pp::Var("{\"status\":\"running\"}"));

	startTime = pp::Module::Get()->core()->GetTimeTicks();
	ticks = 0;
	render(new std::weak_ptr<std::function<void(void*)>>(doRender));

	RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_KEYBOARD);
}

void snygg_instance::update_walls() {
	recording_skin recorder;
	recorder.set_pixels_per_unit(reshaper.get_pixels_per_unit());
	bp->render(recorder);
	walls.update(recorder.recording);
}

static const std::unordered_map<uint32_t, int> key_mapping = {
	{ ' ', game::KEY_SPACE },
	{ 37, game::KEY_LEFT },
	{ 39, game::KEY_RIGHT },
};

bool snygg_instance::handle_key_event(const pp::KeyboardInputEvent& event) {
	if (event.GetModifiers()) return false;

	auto mapping = key_mapping.find(event.GetKeyCode());
	if (mapping == key_mapping.end()) {
		LogToConsole(PP_LOGLEVEL_TIP, pp::Var((double)event.GetKeyCode()));
		return false;
	}

	int keycode = mapping->second;
	bool pressed = event.GetType() == PP_INPUTEVENT_TYPE_KEYDOWN;

	if (key_states[keycode] != pressed) {
		key_states[keycode] = pressed;
		kbd.key_event(keycode, pressed);
	}

	return true;
}

bool snygg_instance::HandleInputEvent(const pp::InputEvent& event) {
	simulate_until(event.GetTimeStamp());

	switch (event.GetType()) {
	case PP_INPUTEVENT_TYPE_KEYDOWN:
    case PP_INPUTEVENT_TYPE_KEYUP:
		return handle_key_event(pp::KeyboardInputEvent(event));

	default:
		return false;
	}
}

void snygg_instance::DidChangeView(const pp::View& view) {
	auto scale = view.GetDeviceScale();
	auto rect = view.GetRect();
	int width = rect.width() * scale, height = rect.height() * scale;

	reshaper.reshape(width, height);
	if (skin) {
		skin->set_transformation(reshaper.get_transformation());
		skin->set_pixels_per_unit(reshaper.get_pixels_per_unit());
	}

	if (context.is_null()) {
		context = initGL(*this, width, height);
		doRender.reset(new std::function<void(void*)>([&](void* userdata){ render(userdata); }));
		maybe_ready();
	} else {
		context.ResizeBuffers(width, height);
		glViewport(0, 0, width, height);
		update_walls();
	}
}

std::map<std::string, std::vector<char>> load_resources(
	pp::InstanceHandle instanceHandle,
	const std::map<std::string, std::string>& resources
) {
	std::map<std::string, std::vector<char>> results;
	std::vector<std::thread> thread_party;
	for (auto& resource : resources) {
		thread_party.emplace_back([&results, resource, instanceHandle] {
			iurlstream in(instanceHandle, resource.second);
			read_entire_stream(results[resource.first], in);
		});
	}
	for (auto& thread : thread_party) thread.join();
	return results;
}

std::map<std::string, image::surface> load_images(
	pp::InstanceHandle instanceHandle,
	const std::map<std::string, std::string>& images
) {
	std::map<std::string, image::surface> results;
	std::vector<std::thread> thread_party;
	for (auto& src : images) {
		thread_party.emplace_back([&results, src, instanceHandle] {
			iurlstream in(instanceHandle, src.second);
			results.emplace(src.first, image::load_jpeg(in));
		});
	}
	for (auto& thread : thread_party) thread.join();
	return results;
}

bool snygg_instance::Init(uint32_t argc, const char* argn[], const char* argv[]) {
	std::map<std::string, std::string> args;
	for (int i=0; i<argc; ++i) args[argn[i]] = argv[i];

	auto boardname = args["board"];
	pp::InstanceHandle instanceHandle{this};

	if (!glInitializePPAPI(pp::Module::Get()->get_browser_interface())) {
		LogToConsole(PP_LOGLEVEL_ERROR, "Unable to initialize GLES PPAPI. Bailing out.");
		return false;
	}


	load_board_thread = async(
		[=]() { return load_board(instanceHandle, boardname); },
		[this](std::pair<std::shared_ptr<board_provider>, std::shared_ptr<board>> stuff) {
			load_board_thread.join();

			std::tie(bpp, bp) = stuff;

			auto bb = bp->bounding_box();
			reshaper.set_box(bb.x1, bb.y1, bb.x2, bb.y2);

			maybe_ready();
		}
	);

	load_resources_thread = async(
		[=]() {
			return load_resources(instanceHandle, {
				{ "color_mapping.glsl", "skins/snakeskin/color_mapping.glsl" },
				{ "flat_fragment.glsl", "skins/snakeskin/flat_fragment.glsl" },
				{ "fragment.glsl", "skins/snakeskin/fragment.glsl" },
				{ "light.glsl", "skins/snakeskin/light.glsl" },
				{ "mb_function.glsl", "skins/snakeskin/mb_function.glsl" },
				{ "mb_mapping.glsl", "skins/snakeskin/mb_mapping.glsl" },
				{ "mb_vertex.glsl", "skins/snakeskin/mb_vertex.glsl" },
				{ "texture_mapping.glsl", "skins/snakeskin/texture_mapping.glsl" },
				{ "vertex.glsl", "skins/snakeskin/vertex.glsl" }
			});
		},
		[this](std::map<std::string, std::vector<char>>&& resources_) {
			load_resources_thread.join();

			resources = std::move(resources_);
			resources_loaded = true;
			maybe_ready();
		}
	);

	load_images_thread = async(
		[=]() {
			return load_images(instanceHandle, {
				{ "diffuse.jpg", "skins/snakeskin/diffuse.jpg" },
				{ "normal.jpg", "skins/snakeskin/normal.jpg" }
			});
		},
		[this](std::map<std::string, image::surface>&& images_) {
			load_images_thread.join();
			images = std::move(images_);
			images_loaded = true;
			maybe_ready();
		}
	);

	return true;
}

void snygg_instance::Graphics3DContextLost() {
	throw std::logic_error("Not implemented: snygg_instance::Graphics3DContextLost()");
}
