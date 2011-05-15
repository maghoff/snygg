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
		GLint lengths[] = { i->source.size() };
		glShaderSource(shader, 1, sources, lengths);
		glCompileShader(shader);
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
