#include <iostream>
#include <limits>
#include <stdexcept>
#include "shader_program.hpp"
#include "shader_builder.hpp"

shader_program::shader_program(const shader_builder& sb) {
	shaders = sb.shaders;
	attrib_locations = sb.attrib_locations;
	program = 0;
}

shader_program::~shader_program() {
	glDeleteProgram(program);
}

void shader_program::recreate_opengl_resources() {
	typedef std::vector<shader_spec>::const_iterator shader_it;
	typedef std::vector<attrib_location_spec>::const_iterator attrib_it;

	glDeleteProgram(program);
	program = glCreateProgram();

	for (shader_it i = shaders.begin(); i != shaders.end(); ++i) {
		GLuint shader = glCreateShader(i->type);
		const GLchar* sources[] = { i->source.data() };

		if (i->source.size() > static_cast<std::size_t>(std::numeric_limits<GLint>::max())) throw std::runtime_error("Soruce code too large");
		GLint lengths[] = { static_cast<GLint>(i->source.size()) };

		glShaderSource(shader, 1, sources, lengths);
		glCompileShader(shader);

		GLint sz;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &sz);
		if (sz > 1) {
			std::string infoLog(sz, char{});
			glGetShaderInfoLog(shader, infoLog.size(), nullptr, &infoLog[0]);
			infoLog.resize(sz - 1);
			std::cerr << "shader compile log:\n" << infoLog << std::endl;
		}

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			glDeleteShader(shader);
			throw std::runtime_error("Shader compile error");
		}

		glAttachShader(program, shader);
	}

	for (attrib_it i = attrib_locations.begin(); i != attrib_locations.end(); ++i) {
		glBindAttribLocation(program, i->index, i->name.c_str());
	}

	glLinkProgram(program);
}

GLuint shader_program::get_program_id() const {
	return program;
}

void shader_program::set_uniform(const GLchar* name, int value) {
	GLuint location = glGetUniformLocation(program, name);
	glUniform1i(location, value);
}

void shader_program::set_uniform(const GLchar* name, float x, float y, float z, float w) {
	GLuint location = glGetUniformLocation(program, name);
	glUniform4f(location, x, y, z, w);
}
