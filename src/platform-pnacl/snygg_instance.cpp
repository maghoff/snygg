#include "snygg_instance.hpp"

#include <ppapi/cpp/var.h>
#include <sstream>
#include <fileutil.hpp>
#include <lua_board_provider.hpp>
#include <board.hpp>
#include <schematic_svg_skin.hpp>
#include <rect.hpp>
#include <complex_polygon.hpp>
#include "urlloader_file_loader.hpp"
#include "iurlstream.hpp"

#include <ppapi/cpp/graphics_3d.h>
#include <GLES2/gl2.h>
#include <ppapi/lib/gl/gles2/gl2ext_ppapi.h>
#include <ppapi/cpp/completion_callback.h>
#include <ppapi/cpp/message_loop.h>

#include <box_reshaper.hpp>
#include <matrix3d.hpp>


std::shared_ptr<board> load_board(pp::InstanceHandle instanceHandle, const std::string& boardname) {
	urlloader_file_loader loader(instanceHandle);

	lua_board_provider board_provider(loader, boardname.length() ? boardname : "wide_screen");
	return std::shared_ptr<board>(new board(board_provider));
}

std::string svg_from_board(const board& b) {
	std::stringstream svgstream;
	{
		schematic_svg_skin svg_skin(svgstream, b.bounding_box());
		svg_skin.enter_state(skin::board_state);
		b.render(svg_skin);
		svg_skin.enter_state(skin::other_state);
		svg_skin.floor(b.floor_polygon());
	}

	return svgstream.str();
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
		auto result = asyncOperation();
		auto postResult = messageLoop.PostWork(
			makeCompletionCallback(
				// FIXME Undesirable copies into capture. Moves would be better.
				[=](int32_t) mutable { callback(std::move(result)); }
			)
		);
		if (postResult != PP_OK) std::terminate();
	});
}


snygg_instance::snygg_instance(PP_Instance instance) :
	Instance(instance),
	Graphics3DClient(this)
{
}


snygg_instance::~snygg_instance() {
	load_board_thread.join();
}

pp::Graphics3D initGL(pp::Instance instance, int32_t new_width, int32_t new_height) {
	if (!glInitializePPAPI(pp::Module::Get()->get_browser_interface())) {
		std::cerr << "Unable to initialize GL PPAPI!\n";
		return pp::Graphics3D();
	}

	const int32_t attrib_list[] = {
		PP_GRAPHICS3DATTRIB_ALPHA_SIZE, 8,
		PP_GRAPHICS3DATTRIB_DEPTH_SIZE, 24,
		PP_GRAPHICS3DATTRIB_WIDTH, new_width,
		PP_GRAPHICS3DATTRIB_HEIGHT, new_height,
		PP_GRAPHICS3DATTRIB_NONE
	};

	pp::Graphics3D context = pp::Graphics3D(pp::InstanceHandle(&instance), attrib_list);

	if (!instance.BindGraphics(context)) {
		std::cerr << "Unable to bind 3d context!\n";
		return pp::Graphics3D();
	}

	glSetCurrentContextPPAPI(context.pp_resource());

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

void snygg_instance::render(void* userdata) {
	if (bp && resources_loaded) {
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, bp->floor_polygon().triangles.size(), GL_UNSIGNED_SHORT, nullptr);

		check_gl_error();
	} else {
		glClearColor(1, 0, 0, 0.2);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	context.SwapBuffers(pp::CompletionCallback(&renderLoopTrampoline, userdata));
}

GLuint compileShader(GLenum shaderType, const std::vector<char>& source, std::string& compileLog) {
	if (source.size() > std::numeric_limits<GLint>::max()) throw std::runtime_error("Shader source way too big");

	auto shader = glCreateShader(shaderType);
	const GLchar * sources[] = { source.data() };
	GLint lengths[] = { static_cast<GLint>(source.size()) };
	glShaderSource(shader, 1, sources, lengths);
	glCompileShader(shader);

	GLint sz;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &sz);
	compileLog.resize(sz);
	glGetShaderInfoLog(shader, compileLog.size(), nullptr, &compileLog[0]);
	if (sz > 0) compileLog.resize(sz-1);

	return shader;
}

void snygg_instance::maybe_ready() {
	if (!bp) return;
	if (!resources_loaded) return;

	GLboolean shaderCompilerSupported;
	glGetBooleanv(GL_SHADER_COMPILER, &shaderCompilerSupported);
	if (shaderCompilerSupported == GL_FALSE) throw std::runtime_error("Shader compiler not supported");


	std::string compileLog;
	auto vert = compileShader(GL_VERTEX_SHADER, resources["mb_vertex.glsl"], compileLog);
	if (compileLog.length()) {
		LogToConsole(PP_LOGLEVEL_LOG, pp::Var("vertex shader compile log:"));
		LogToConsole(PP_LOGLEVEL_LOG, pp::Var(compileLog));
	}

	auto fragmentSource = resources["light.glsl"];
	fragmentSource.insert(fragmentSource.end(), resources["flat_fragment.glsl"].begin(), resources["flat_fragment.glsl"].end());
	auto frag = compileShader(GL_FRAGMENT_SHADER, fragmentSource, compileLog);
	if (compileLog.length()) {
		LogToConsole(PP_LOGLEVEL_LOG, pp::Var("fragment shader compile log:"));
		LogToConsole(PP_LOGLEVEL_LOG, pp::Var(compileLog));
	}


	auto program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);

	const auto vertexLocation = 0;
	glBindAttribLocation(program, vertexLocation, "vertex");

	glLinkProgram(program);
	glUseProgram(program);


	auto& floor = bp->floor_polygon();

	GLuint vertexVBO;
	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);

	std::vector<GLfloat> points;
	for (auto& p : floor.points) {
		points.push_back(p.x());
		points.push_back(p.y());
	}
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), points.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	std::vector<uint16_t> indices(floor.triangles.begin(), floor.triangles.end());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);


	auto bb = bp->bounding_box();
	game::box_reshaper reshaper;
	reshaper.set_box(bb.x1, bb.y1, bb.x2, bb.y2);
	reshaper.reshape(850*2, 510*2);
	auto transform = reshaper.get_transformation().transposed();
	auto transformLocation = glGetUniformLocation(program, "transform");
	glUniformMatrix3fv(transformLocation, 1, GL_FALSE, transform.v);


	glUniform4f(glGetUniformLocation(program, "ambient"), 0.4f, 0.4f, 0.4f, 1.0f);
	glUniform4f(glGetUniformLocation(program, "diffuse"), 0.5f, 0.5f, 0.5f, 1.0f);


	check_gl_error();
	LogToConsole(PP_LOGLEVEL_LOG, pp::Var("GLES initialized"));
}

void snygg_instance::DidChangeView(const pp::View& view) {
	// TODO Consider view.GetDeviceScale() ?
	auto rect = view.GetRect();
}

std::map<std::string, std::vector<char>> load_resources(
	pp::InstanceHandle instanceHandle,
	const std::map<std::string, std::string>& resources
) {
	std::map<std::string, std::vector<char>> results;
	for (auto& resource : resources) {
		iurlstream in(instanceHandle, resource.second);
		read_entire_stream(results[resource.first], in);
	}
	return results;
}

bool snygg_instance::Init(uint32_t argc, const char* argn[], const char* argv[]) {
	std::map<std::string, std::string> args;
	for (int i=0; i<argc; ++i) args[argn[i]] = argv[i];

	auto boardname = args["board"];
	pp::InstanceHandle instanceHandle{this};

	load_board_thread = async(
		[=]() { return load_board(instanceHandle, boardname); },
		[this](std::shared_ptr<board> bp_) {
			load_board_thread.join();

			bp.swap(bp_);
			PostMessage(pp::Var(svg_from_board(*bp)));
			maybe_ready();
		}
	);

	load_resources_thread = async(
		[=]() {
			return load_resources(instanceHandle, {
				{ "color_mapping.glsl", "skins/snakeskin/color_mapping.glsl" },
				{ "diffuse.jpg", "skins/snakeskin/diffuse.jpg" },
				{ "flat_fragment.glsl", "skins/snakeskin/flat_fragment.glsl" },
				{ "fragment.glsl", "skins/snakeskin/fragment.glsl" },
				{ "light.glsl", "skins/snakeskin/light.glsl" },
				{ "mb_function.glsl", "skins/snakeskin/mb_function.glsl" },
				{ "mb_mapping.glsl", "skins/snakeskin/mb_mapping.glsl" },
				{ "mb_vertex.glsl", "skins/snakeskin/mb_vertex.glsl" },
				{ "flat_vertex.glsl", "skins/snakeskin/flat_vertex.glsl" },
				{ "normal.jpg", "skins/snakeskin/normal.jpg" },
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

	context = initGL(*this, 850*2, 510*2);
	doRender.reset(new std::function<void(void*)>([&](void* userdata){ render(userdata); }));
	render(new std::weak_ptr<std::function<void(void*)>>(doRender));

	return true;
}

void snygg_instance::Graphics3DContextLost() {
	throw std::logic_error("Not implemented: snygg_instance::Graphics3DContextLost()");
}
