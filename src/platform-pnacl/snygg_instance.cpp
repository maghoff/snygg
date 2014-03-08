#include "snygg_instance.hpp"

#include <sstream>
#include <fileutil.hpp>
#include <lua_board_provider.hpp>
#include <board.hpp>
#include <schematic_svg_skin.hpp>
#include <rect.hpp>
#include <complex_polygon.hpp>
#include "urlloader_file_loader.hpp"
#include "iurlstream.hpp"
#include "ologstream.hpp"

#include <ppapi/cpp/graphics_3d.h>
#include <GLES2/gl2.h>
#include <ppapi/lib/gl/gles2/gl2ext_ppapi.h>
#include <ppapi/cpp/completion_callback.h>
#include <ppapi/cpp/message_loop.h>
#include <ppapi/cpp/var.h>

#include <matrix3d.hpp>


namespace attrib {
	enum {
		vertex = 0,
		circle_coord,
		across,
		along,
		b
	};
}


std::shared_ptr<board> load_board(pp::InstanceHandle instanceHandle, const std::string& boardname) {
	urlloader_file_loader loader(instanceHandle);

	lua_board_provider board_provider(loader, boardname.length() ? boardname : "wide_screen");
	return std::shared_ptr<board>(new board(board_provider));
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
	Graphics3DClient(this),
	lout(*this, PP_LOGLEVEL_LOG)
{
}


snygg_instance::~snygg_instance() {
	load_board_thread.join();
}

pp::Graphics3D initGL(pp::Instance instance, int32_t new_width, int32_t new_height) {
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
	auto transform = reshaper.get_transformation().transposed();

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);


	glUseProgram(floorProgram);

	glUniformMatrix3fv(glGetUniformLocation(floorProgram, "transform"), 1, GL_FALSE, transform.v);

	glUniform4f(glGetUniformLocation(floorProgram, "ambient"), 0.4f, 0.4f, 0.4f, 1.0f);
	glUniform4f(glGetUniformLocation(floorProgram, "diffuse"), 0.5f, 0.5f, 0.5f, 1.0f);

	floor.render(attrib::vertex);


	glUseProgram(colorProgram);

	glUniformMatrix3fv(glGetUniformLocation(colorProgram, "transform"), 1, GL_FALSE, transform.v);

	glUniform4f(glGetUniformLocation(colorProgram, "ambient"), 0.4f, 0.4f, 0.4f, 1.0f);
	glUniform4f(glGetUniformLocation(colorProgram, "diffuse"), 0.5f, 0.5f, 0.5f, 1.0f);

	bp->render(skin);


	glUseProgram(0);


	context.SwapBuffers(pp::CompletionCallback(&renderLoopTrampoline, userdata));
}

GLuint compileShader(GLenum shaderType, const std::vector<std::vector<char>>& source, std::ostream& out) {
	if (source.size() > std::numeric_limits<GLsizei>::max()) throw std::runtime_error("Shader source way too big");

	std::vector<const GLchar*> sources;
	std::vector<GLint> lengths;
	for (auto& source_fragment : source) {
		if (source_fragment.size() > std::numeric_limits<GLint>::max()) throw std::runtime_error("Shader source way too big");

		sources.emplace_back(source_fragment.data());
		lengths.emplace_back(source_fragment.size());
	}

	auto shader = glCreateShader(shaderType);

	glShaderSource(shader, source.size(), sources.data(), lengths.data());
	glCompileShader(shader);

	GLint sz;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &sz);
	if (sz > 1) {
		std::string infoLog(sz, char{});
		glGetShaderInfoLog(shader, infoLog.size(), nullptr, &infoLog[0]);
		infoLog.resize(sz - 1);
		out << "shader compile log:\n" << infoLog << std::endl;
	}

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		glDeleteShader(shader);
		shader = 0;
	}

	return shader;
}

GLuint buildShaderProgram(
	const std::vector<std::pair<GLenum, std::vector<std::vector<char>>>>& shaders,
	const std::vector<std::pair<std::string, GLuint>>& attribs,
	std::ostream& out
) {
	auto program = glCreateProgram();

	for (auto& shader : shaders) {
		auto shaderId = compileShader(shader.first, shader.second, out);

		if (!shaderId) {
			glDeleteProgram(program);
			return 0;
		}

		glAttachShader(program, shaderId);
		glDeleteShader(shaderId);
	}

	for (auto& attrib : attribs) glBindAttribLocation(program, attrib.second, attrib.first.c_str());
	glLinkProgram(program);

	GLint sz;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &sz);
	if (sz > 1) {
		std::string infoLog(sz, char{});
		glGetProgramInfoLog(program, infoLog.size(), nullptr, &infoLog[0]);
		infoLog.resize(sz-1);
		out << "program link log:\n" << infoLog << std::endl;
	}

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (success == GL_FALSE) {
		glDeleteProgram(program);
		program = 0;
	}

	return program;
}


void snygg_instance::maybe_ready() {
	if (!bp) return;
	if (!resources_loaded) return;
	if (context.is_null()) return;


	floorProgram = buildShaderProgram(
		{
			{ GL_VERTEX_SHADER, { resources["mb_vertex.glsl"] } },
			{ GL_FRAGMENT_SHADER, { resources["light.glsl"], resources["flat_fragment.glsl"] } }
		}, {
			{ "vertex", attrib::vertex }
		},
		lout
	);
	if (floorProgram == 0) throw std::runtime_error("Shader linking failed");


	colorProgram = buildShaderProgram(
		{
			{ GL_VERTEX_SHADER, { resources["vertex.glsl"] } },
			{ GL_FRAGMENT_SHADER, {
				resources["light.glsl"],
				resources["fragment.glsl"],
				resources["color_mapping.glsl"]
			} }
		}, {
			{ "vertex", attrib::vertex },
			{ "circle_coord", attrib::circle_coord },
			{ "across_in", attrib::across },
			{ "along_in", attrib::along },
			{ "b_in", attrib::b }
		},
		lout
	);
	if (colorProgram == 0) throw std::runtime_error("Shader linking failed");


	floor = std::move(renderable_complex_polygon(bp->floor_polygon()));


	check_gl_error();

	PostMessage(pp::Var("{\"status\":\"running\"}"));

	render(new std::weak_ptr<std::function<void(void*)>>(doRender));
}

void snygg_instance::DidChangeView(const pp::View& view) {
	auto scale = view.GetDeviceScale();
	auto rect = view.GetRect();
	int width = rect.width() * scale, height = rect.height() * scale;

	reshaper.reshape(width, height);
	skin.set_transformation(reshaper.get_transformation());
	skin.set_pixels_per_unit(reshaper.get_pixels_per_unit());

	if (context.is_null()) {
		context = initGL(*this, width, height);
		glViewport(0, 0, width, height);
		doRender.reset(new std::function<void(void*)>([&](void* userdata){ render(userdata); }));
		maybe_ready();
	} else {
		context.ResizeBuffers(width, height);
		glViewport(0, 0, width, height);
	}
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

	if (!glInitializePPAPI(pp::Module::Get()->get_browser_interface())) {
		LogToConsole(PP_LOGLEVEL_ERROR, "Unable to initialize GLES PPAPI. Bailing out.");
		return false;
	}

	GLboolean shaderCompilerSupported;
	glGetBooleanv(GL_SHADER_COMPILER, &shaderCompilerSupported);
	if (shaderCompilerSupported == GL_FALSE) {
		LogToConsole(PP_LOGLEVEL_ERROR, "GLSL shader compiler not supported. Bailing out.");
		return false;
	}


	load_board_thread = async(
		[=]() { return load_board(instanceHandle, boardname); },
		[this](std::shared_ptr<board> bp_) {
			load_board_thread.join();

			bp.swap(bp_);

			auto bb = bp->bounding_box();
			reshaper.set_box(bb.x1, bb.y1, bb.x2, bb.y2);
			skin.set_transformation(reshaper.get_transformation());
			skin.set_pixels_per_unit(reshaper.get_pixels_per_unit());

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

	return true;
}

void snygg_instance::Graphics3DContextLost() {
	throw std::logic_error("Not implemented: snygg_instance::Graphics3DContextLost()");
}
