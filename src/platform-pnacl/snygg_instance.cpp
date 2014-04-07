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
#include "metaballs.hpp"

#include "nacl_errors.hpp"


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
		if (postResult != PP_OK) fail(err::postwork_failed, "messageLoop.PostWork did not return PP_OK, but rather " + std::to_string(postResult));
	});
}


snygg_instance::snygg_instance(PP_Instance instance) :
	Instance(instance),
	Graphics3DClient(this),
	lout(*this, PP_LOGLEVEL_LOG),
	lerr(*this, PP_LOGLEVEL_ERROR)
{
}


snygg_instance::~snygg_instance() {
	load_board_thread.join();
	load_resources_thread.join();
	load_images_thread.join();
}

void snygg_instance::add_renderable(std::unique_ptr<renderable>&& r) {
	renderables.emplace_back(std::move(r));
	render();
}

void snygg_instance::add_crashable(std::unique_ptr<crashable>&& r) {
	new_crashables.emplace_back(std::move(r));
	render();
}

void snygg_instance::add_movable(std::unique_ptr<movable>&& r) {
	movables.emplace_back(std::move(r));
	render();
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

	if (context.is_null()) fail(err::unable_to_create_3d_context, "Unable to create 3d context");
	if (!instance.BindGraphics(context)) fail(err::unable_to_bind_3d_context, "Unable to bind 3d context");

	glSetCurrentContextPPAPI(context.pp_resource());

	GLboolean shaderCompilerSupported;
	glGetBooleanv(GL_SHADER_COMPILER, &shaderCompilerSupported);
	if (shaderCompilerSupported == GL_FALSE) {
		fail(err::glsl_shader_not_supported, "GLSL shader compiler not supported");
	}

	return context;
}

void snygg_instance::check_gl_error() {
	auto status = glGetError();
	if (status != GL_NO_ERROR) {
		LogToConsole(PP_LOGLEVEL_ERROR, pp::Var((int32_t)status));
	}
}

template <typename Container>
static void cleanup_dead(Container& c) {
	auto new_end = std::remove_if(std::begin(c), std::end(c), [](const typename Container::value_type& i) { return i->is_dead(); });
	c.erase(new_end, std::end(c));
}

template <typename Container>
static void add_new_items(Container& target, Container& source) {
	target.reserve(target.size() + source.size());
	for (auto& item : source) target.emplace_back(std::move(item));
	source.clear();
}

void snygg_instance::tick_10ms() {
	add_new_items(crashables, new_crashables);

	for (auto& movable : movables) {
		if (!movable->is_dead()) movable->move();
	}

	for (auto& player : players) {
		for (auto& crashable : crashables) {
			if (!crashable->is_dead() && player->crashes_with(*crashable)) crashable->hit_by(*player);
		}
		for (auto& movable : movables) {
			if (!movable->is_dead() && player->crashes_with(*movable)) movable->hit_by(*player);
		}
		if (player->crashes_with(*bp)) player->die();
	}

	cleanup_dead(crashables);
	cleanup_dead(movables);

	add_new_items(crashables, new_crashables);
}

void snygg_instance::simulate_until(PP_TimeTicks nowTimeTicks) {
	int now = (nowTimeTicks - startTime) * 100;

	if (now > ticks + 200) ticks = now;

	while (ticks < now) {
		tick_10ms();
		++ticks;
	}
}

void snygg_instance::score_updated(int score) {
	PostMessage(pp::Var("{"
		"\"what\":\"score_updated\","
		"\"score\":" + std::to_string(score) + ","
		"\"board\":\"" + current_board_name + "\""
	"}"));
}

void snygg_instance::died(int score) {
	PostMessage(pp::Var("{"
		"\"what\":\"died\","
		"\"score\":" + std::to_string(score) + ","
		"\"board\":\"" + current_board_name + "\""
	"}"));
}

void render_callback_trampoline(void* userdata, int32_t status) {
	auto instance = static_cast<snygg_instance*>(userdata);
	instance->render_callback(status);
}

void snygg_instance::render_callback(int32_t status) {
	simulate_until(pp::Module::Get()->core()->GetTimeTicks());
	render_callback_pending = false;
	if (status != PP_OK) return;
	if (should_render) render();
}

void snygg_instance::render() {
	if (render_callback_pending) {
		should_render = true;
		return;
	}
	render_callback_pending = true;
	should_render = !movables.empty();


	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	skin->floor(floor);


	for (auto& renderable : renderables) renderable->render(*metaballs);
	metaballs->floor(floor);


	skin->enter_state(skin::other_state);
	for (auto& movable : movables) movable->render(*skin);
	for (auto& crashable : crashables) crashable->render(*skin);
	for (auto& renderable : renderables) renderable->render(*skin);

	skin->enter_state(skin::board_state);

	walls.render(*skin);


	context.SwapBuffers(pp::CompletionCallback(&render_callback_trampoline, this));
}

#include <iostream>
std::ostream& operator << (std::ostream& out, const la::matrix33f& m) {
	out << "[ ";
	for (int row = 0; row < m.rows; ++row) {
		if (row != 0) out << " ; ";
		for (int col = 0; col < m.cols; ++col) {
			if (col != 0) out << ' ';
			out << m[row][col];
		}
	}
	out << " ]";

	return out;
}

void snygg_instance::maybe_ready() {
	lout << "maybe_ready()" << std::endl;
	if (!bp) { lout << "not ready: !bp" << std::endl; return; }
	if (!resources_loaded) { lout << "not ready: !resources_loaded" << std::endl; return; }
	if (!images_loaded) { lout << "not ready: !images_loaded" << std::endl; return; }
	if (context.is_null()) { lout << "not ready: context.is_null()" << std::endl; return; }
	lout << "ready!" << std::endl;
	ready = true;


	lout << "  skin.reset()" << std::endl;
	skin.reset(new buffering_skin(resources, images, lout));
	lout << "    set_transformation" << std::endl;
	lout << "      reshaper.get_transformation(): " << reshaper.get_transformation() << std::endl;
	skin->set_transformation(reshaper.get_transformation());
	lout << "    set_pixels_per_unit" << std::endl;
	lout << "      reshaper.get_pixels_per_unit(): " << reshaper.get_pixels_per_unit() << std::endl;
	skin->set_pixels_per_unit(reshaper.get_pixels_per_unit());
	lout << "    load_opengl_resources" << std::endl;
	skin->load_opengl_resources(storedWidth, storedHeight);
	lout << "    ---" << std::endl;

	lout << "  metaballs.reset()" << std::endl;
	metaballs.reset(new class metaballs(resources, lout));
	metaballs->set_transformation(reshaper.get_transformation());
	metaballs->set_pixels_per_unit(reshaper.get_pixels_per_unit());
	metaballs->load_opengl_resources(storedWidth, storedHeight);

	lout << "  renderable_complex_polygon()" << std::endl;
	floor = std::move(renderable_complex_polygon(bp->floor_polygon()));

	lout << "  update_walls()" << std::endl;
	update_walls();

	lout << "  check_gl_error()" << std::endl;
	check_gl_error();


	lout << "  *.clear()" << std::endl;
	players.clear();
	renderables.clear();
	crashables.clear();
	movables.clear();


	lout << "  bp->get_starting_position()" << std::endl;
	bp->get_starting_position();
	players.emplace_back(new player(
		kbd, *this, *bp, *this,
		game::KEY_LEFT, game::KEY_RIGHT, game::KEY_SPACE
	));


	lout << "  fg.reset()" << std::endl;
	fg.reset(new food_generator(*this, *bp));
	fg->generate();
	add_new_items(crashables, new_crashables);


	lout << "  PostMessage()" << std::endl;
	PostMessage(pp::Var("{\"what\":\"status\",\"status\":\"running\"}"));

	lout << "  core()->GetTimeTicks()" << std::endl;
	startTime = pp::Module::Get()->core()->GetTimeTicks();
	ticks = 0;
	lout << "calling render()" << std::endl;
	render();

	lout << "  RequestInputEvents()" << std::endl;
	RequestInputEvents(PP_INPUTEVENT_CLASS_MOUSE);
	lout << "  RequestFilteringInputEvents()" << std::endl;
	RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_KEYBOARD);

	lout << "maybe_ready() finished" << std::endl;
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
	bool pressed = event.GetType() == PP_INPUTEVENT_TYPE_KEYDOWN;
	const auto modifiers_to_ignore =
		PP_INPUTEVENT_MODIFIER_SHIFTKEY |
		PP_INPUTEVENT_MODIFIER_CONTROLKEY |
		PP_INPUTEVENT_MODIFIER_ALTKEY |
		PP_INPUTEVENT_MODIFIER_METAKEY
	;
	if (pressed && (event.GetModifiers() & modifiers_to_ignore)) {
		lout << "Input event ignored due to modifier keys" << std::endl;
		return false;
	}

	auto mapping = key_mapping.find(event.GetKeyCode());
	if (mapping == key_mapping.end()) return false;

	int keycode = mapping->second;

	if (key_states[keycode] != pressed) {
		key_states[keycode] = pressed;
		kbd.key_event(keycode, pressed);
	}

	return true;
}

bool snygg_instance::handle_keypress_event(const pp::KeyboardInputEvent& event) {
	if (event.GetModifiers()) return false;

	auto ch = event.GetCharacterText().AsString();
	if (ch == " ") return true;

	return false;
}

bool snygg_instance::HandleInputEvent(const pp::InputEvent& event) {
	// event.GetTimeStamp() seems to get out of sync with pp::Module::Get()->core()->GetTimeTicks()
	// upon system sleep. See http://stackoverflow.com/questions/22688934/
	simulate_until(pp::Module::Get()->core()->GetTimeTicks());

	switch (event.GetType()) {
	case PP_INPUTEVENT_TYPE_KEYDOWN:
    case PP_INPUTEVENT_TYPE_KEYUP:
		return handle_key_event(pp::KeyboardInputEvent(event));

	case PP_INPUTEVENT_TYPE_CHAR:
		return handle_keypress_event(pp::KeyboardInputEvent(event));

	case PP_INPUTEVENT_TYPE_MOUSEDOWN:
	case PP_INPUTEVENT_TYPE_MOUSEUP:
	case PP_INPUTEVENT_TYPE_MOUSEENTER:
	case PP_INPUTEVENT_TYPE_MOUSELEAVE:
	case PP_INPUTEVENT_TYPE_MOUSEMOVE:
		return true;

	default:
		return false;
	}
}

void snygg_instance::load_board(const std::string& new_board_name) {
	loading_board_name = new_board_name;

	if (loading_board_name == current_board_name) return;

	if (load_board_thread.joinable()) return;

	auto instanceHandle = pp::InstanceHandle(this);
	load_board_thread = async(
		[=]() { return ::load_board(instanceHandle, new_board_name); },
		[=](std::pair<std::shared_ptr<board_provider>, std::shared_ptr<board>> stuff) {
			lout << "load_board called back" << std::endl;

			load_board_thread.join();
			load_board_thread = std::thread();

			if (loading_board_name != new_board_name) {
				load_board(loading_board_name);
				return;
			}

			std::tie(bpp, bp) = stuff;
			current_board_name = new_board_name;

			auto bb = bp->bounding_box();
			reshaper.set_box(bb.x1, bb.y1, bb.x2, bb.y2);

			maybe_ready();
		}
	);
}

void snygg_instance::HandleMessage(const pp::Var& var_message) {
	if (!var_message.is_string()) return;
	std::string new_board_name = var_message.AsString();

	lout << "nacl got message: " << new_board_name << std::endl;

	load_board(new_board_name);
}

void snygg_instance::DidChangeView(const pp::View& view) {
	auto scale = view.GetDeviceScale();
	auto rect = view.GetRect();
	int width = rect.width() * scale, height = rect.height() * scale;
	if (storedWidth == width && storedHeight == height) return;
	storedWidth = width;
	storedHeight = height;

	lout << "DidChangeView (size actually changed)" << std::endl;

	reshaper.reshape(width, height);
	if (skin) {
		lout << "Updating skins. context.is_null(): " << std::boolalpha << context.is_null() << std::endl;
		skin->set_transformation(reshaper.get_transformation());
		skin->set_pixels_per_unit(reshaper.get_pixels_per_unit());
		skin->load_opengl_resources(width, height);
		metaballs->set_transformation(reshaper.get_transformation());
		metaballs->set_pixels_per_unit(reshaper.get_pixels_per_unit());
		metaballs->load_opengl_resources(width, height);
	}

	if (context.is_null()) {
		lout << "context = initGL" << std::endl;
		context = initGL(*this, width, height);
		maybe_ready();
	} else {
		lout << "context.ResizeBuffers(" << width << ", " << height << ")" << std::endl;
		context.ResizeBuffers(width, height);
		glViewport(0, 0, width, height);
		if (bp) update_walls();
		if (ready) render();
	}
}

std::map<std::string, std::vector<char>> load_resources(
	pp::InstanceHandle instanceHandle,
	const std::map<std::string, std::string>& resources
) {
	std::map<std::string, std::vector<char>> results;
	std::vector<std::thread> thread_party;

	// Allocate entire map, so we don't mess with the *map*
	// in the subthreads. #threadworldproblems
	for (auto& resource : resources) results[resource.first];

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
			lout << "load_resources called back" << std::endl;
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
			lout << "load_images called back" << std::endl;
			load_images_thread.join();
			images = std::move(images_);
			images_loaded = true;
			maybe_ready();
		}
	);

	lout << "Init() completed" << std::endl;

	return true;
}

void snygg_instance::Graphics3DContextLost() {
	fail(err::graphics3d_context_lost, "Not implemented: snygg_instance::Graphics3DContextLost()");
}
