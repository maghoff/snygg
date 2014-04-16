#include "buffering_skin.hpp"
#include <cassert>
#include <ostream>
#include <GLES2/gl2.h>
#include <load_jpeg.hpp>
#include <surface.hpp>
#include "geometry_spec.hpp"
#include "geometry_builder.hpp"
#include "renderable_complex_polygon.hpp"
#include "nacl_errors.hpp"

namespace attrib {
	enum {
		vertex = 0,
		circle_coord,
		across,
		along,
		b
	};
}

const la::vec4f ambient(.2f, .2f, .2f, 1.f);


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


template <typename T>
static const T& get(const std::map<std::string, T>& resources, const std::string& key) {
	auto i = resources.find(key);
	if (i == resources.end()) fail(err::missing_resource, "Requested missing resource");
	return i->second;
}


static void copy_surface_to_texture(const image::surface& s, unsigned int gl_texture) {
	GLuint format = s.components() == 4 ? GL_RGBA : GL_RGB;

	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, format, s.width(), s.height(), 0, format, GL_UNSIGNED_BYTE, s.data().data());
	glGenerateMipmap(GL_TEXTURE_2D);
}

int buffering_skin::acquire_fallback_shader(const std::map<std::string, std::vector<char>>& resources) {
	out << "Shader linking failed. Attempting fallback shader." << std::endl;

	if (fallbackProgram) {
		out << "Returning cached fallbackProgram" << std::endl;
		return fallbackProgram;
	}

	out << "Compiling fallbackProgram" << std::endl;
	fallbackProgram = buildShaderProgram(
		{
			{ GL_VERTEX_SHADER, { get(resources, "flat/vertex.glsl") } },
			{ GL_FRAGMENT_SHADER, { get(resources, "flat/fragment.glsl") } }
		}, {
			{ "vertex", attrib::vertex }
		},
		out
	);

	if (fallbackProgram == 0) fail(err::shader_linking_failed, "Shader linking failed");

	return fallbackProgram;
}

buffering_skin::buffering_skin(
	const std::map<std::string, std::vector<char>>& resources,
	const std::map<std::string, image::surface>& images,
	std::ostream& out_
) : out(out_), buffer(0) {
	out << "> buffering_skin::buffering_skin" << std::endl;

	out << "floorProgram" << std::endl;
	floorProgram = buildShaderProgram(
		{
			{ GL_VERTEX_SHADER, { get(resources, "mb_vertex.glsl") } },
			{ GL_FRAGMENT_SHADER, { get(resources, "light.glsl"), get(resources, "flat_fragment.glsl") } }
		}, {
			{ "vertex", attrib::vertex }
		},
		out
	);
	if (floorProgram == 0) floorProgram = acquire_fallback_shader(resources);


	out << "colorProgram" << std::endl;
	colorProgram = buildShaderProgram(
		{
			{ GL_VERTEX_SHADER, { get(resources, "vertex.glsl") } },
			{ GL_FRAGMENT_SHADER, {
				get(resources, "light.glsl"),
				get(resources, "fragment.glsl"),
				get(resources, "color_mapping.glsl")
			} }
		}, {
			{ "vertex", attrib::vertex },
			{ "circle_coord", attrib::circle_coord },
			{ "across_in", attrib::across },
			{ "along_in", attrib::along },
			{ "b_in", attrib::b }
		},
		out
	);
	if (colorProgram == 0) colorProgram = acquire_fallback_shader(resources);


	out << "textureProgram" << std::endl;
	textureProgram = buildShaderProgram(
		{
			{ GL_VERTEX_SHADER, { get(resources, "vertex.glsl") } },
			{ GL_FRAGMENT_SHADER, {
				get(resources, "light.glsl"),
				get(resources, "fragment.glsl"),
				get(resources, "texture_mapping.glsl")
			} }
		}, {
			{ "vertex", attrib::vertex },
			{ "circle_coord", attrib::circle_coord },
			{ "across_in", attrib::across },
			{ "along_in", attrib::along },
			{ "b_in", attrib::b }
		},
		out
	);
	if (textureProgram == 0) textureProgram = acquire_fallback_shader(resources);

	out << "genTextures" << std::endl;
	glGenTextures(2, textures);
	out << "diffuse.jpg" << std::endl;
	copy_surface_to_texture(get(images, "diffuse.jpg"), textures[0]);
	out << "normal.jpg" << std::endl;
	copy_surface_to_texture(get(images, "normal.jpg"), textures[1]);

	out << "< buffering_skin::buffering_skin" << std::endl;
}

buffering_skin::~buffering_skin() {
	if (buffer != 0) glDeleteBuffers(1, &buffer);
	glDeleteTextures(2, textures);
}

void buffering_skin::to_floor_shader() {
	glUseProgram(floorProgram);

	glUniformMatrix3fv(glGetUniformLocation(floorProgram, "transform"), 1, GL_FALSE, transform.v);

	glUniform4fv(glGetUniformLocation(floorProgram, "ambient"), 1, ambient.v);
	glUniform4f(glGetUniformLocation(floorProgram, "diffuse"), 0.f, 0.f, 0.f, 1.0f);
	glUniform4f(glGetUniformLocation(floorProgram, "color"), 0.f, 0.f, 0.f, 1.0f);
}

void buffering_skin::to_wall_shader() {
	glUseProgram(colorProgram);

	glUniformMatrix3fv(glGetUniformLocation(colorProgram, "transform"), 1, GL_FALSE, transform.v);

	glUniform4fv(glGetUniformLocation(colorProgram, "ambient"), 1, ambient.v);
	glUniform4f(glGetUniformLocation(colorProgram, "color"), 0.2f, 0.2f, 0.2f, 1.0f);
}

void buffering_skin::to_texture_shader() {
	glUseProgram(textureProgram);

	glUniformMatrix3fv(glGetUniformLocation(textureProgram, "transform"), 1, GL_FALSE, transform.v);

	glUniform4fv(glGetUniformLocation(textureProgram, "ambient"), 1, ambient.v);
	glUniform4f(glGetUniformLocation(textureProgram, "color"), 0.2f, 0.4f, 0.2f, 1.0f);
	glUniform1i(glGetUniformLocation(textureProgram, "diffuse_map"), 0);
	glUniform1i(glGetUniformLocation(textureProgram, "normal_map"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void buffering_skin::draw_geometry_spec(const geometry_spec& spec) {
	if (buffer == 0) glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, spec.data.size() * sizeof(spec.data[0]), spec.data.data(), GL_STREAM_DRAW);

	draw_arrays(buffer, spec.data.size(), spec.mode);
}

void buffering_skin::draw_arrays(unsigned buffer_object, unsigned n, geometry_mode mode) {
	glBindBuffer(GL_ARRAY_BUFFER, buffer_object);

	glEnableVertexAttribArray(attrib::vertex);
	glEnableVertexAttribArray(attrib::across);
	glEnableVertexAttribArray(attrib::along);
	glEnableVertexAttribArray(attrib::circle_coord);
	glEnableVertexAttribArray(attrib::b);

	auto stride = sizeof(vertex_spec);
	glVertexAttribPointer(attrib::vertex, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, vertex));
	glVertexAttribPointer(attrib::across, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, across));
	glVertexAttribPointer(attrib::along, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, along));
	glVertexAttribPointer(attrib::circle_coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, circle_coord));
	glVertexAttribPointer(attrib::b, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, b));

	glDrawArrays(mode == geometry_mode::triangle_fan ? GL_TRIANGLE_FAN : GL_TRIANGLE_STRIP, 0, n);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void buffering_skin::draw_elements(unsigned int vertex_buffer_object, unsigned int index_buffer_object, unsigned int elements) {
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

	glEnableVertexAttribArray(attrib::vertex);
	glEnableVertexAttribArray(attrib::across);
	glEnableVertexAttribArray(attrib::along);
	glEnableVertexAttribArray(attrib::circle_coord);
	glEnableVertexAttribArray(attrib::b);

	auto stride = sizeof(vertex_spec);
	glVertexAttribPointer(attrib::vertex, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, vertex));
	glVertexAttribPointer(attrib::across, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, across));
	glVertexAttribPointer(attrib::along, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, along));
	glVertexAttribPointer(attrib::circle_coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, circle_coord));
	glVertexAttribPointer(attrib::b, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, b));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
	glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_SHORT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void buffering_skin::load_opengl_resources(int width, int height) {
}

void buffering_skin::set_transformation(const la::matrix33f& transform_) {
	transform = transform_.transposed();
}

void buffering_skin::circle(la::vec2f p, float r) {
	to_wall_shader();
	glUniform4f(glGetUniformLocation(colorProgram, "color"), 0.6f, 0.4f, 0.2f, 1.0f);
	draw_geometry_spec(
		geometry_builder::circle(
			[&](float r) -> float { return get_step_size(r); },
			p, r
		)
	);
	to_texture_shader();
}

void buffering_skin::blood(la::vec2f p, float r) {
}

void buffering_skin::fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
	draw_geometry_spec(
		geometry_builder::fat_arc(
			[&](float r) -> float { return get_step_size(r); },
			p, r, t, begin, end, b_begin, b_end
		)
	);
}

void buffering_skin::fat_line(la::vec2f p, la::vec2f dir, float len, float t, float b_begin, float b_end) {
	draw_geometry_spec(geometry_builder::fat_line(p, dir, len, t, b_begin, b_end));
}

void buffering_skin::cap(la::vec2f p, float snake_direction_in, float cap_direction_in, float b_coord) {
	draw_geometry_spec(
		geometry_builder::cap(
			[&](float r) -> float { return get_step_size(r); },
			p, snake_direction_in, cap_direction_in, b_coord
		)
	);
}

void buffering_skin::floor(const complex_polygon& floor_poly) {
	throw std::logic_error("Wrong buffering_skin::floor called. Use renderable_complex_polygon overload!");
}

void buffering_skin::floor(const renderable_complex_polygon& floor) {
	to_floor_shader();
	floor.render(attrib::vertex);
}

void buffering_skin::enter_state(state_t st) {
	switch (st) {
	case board_state:
		to_wall_shader();
		break;
	case other_state:
		to_texture_shader();
		break;
	}
}
