#include <ymse/gl.h>
#include "shader_configuration.hpp"
#include "shader_program.hpp"

struct uniform_setter {
	// All the subclasses have trivial destructors
	//virtual ~uniform_setter();

	virtual void set_to(GLuint index) const = 0;
};

class uniform_setter_1i : public uniform_setter {
	int v[1];

public:
	uniform_setter_1i(int v);

	void set_to(GLuint index) const;
};

uniform_setter_1i::uniform_setter_1i(int v_) {
	v[0] = v_;
}

void uniform_setter_1i::set_to(GLuint index) const {
	glUniform1i(index, v[0]);
}

class uniform_setter_4f : public uniform_setter {
	float v[4];

public:
	uniform_setter_4f(float, float, float, float);

	void set_to(GLuint index) const;
};

uniform_setter_4f::uniform_setter_4f(float v0, float v1, float v2, float v3) {
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;
	v[3] = v3;
}

void uniform_setter_4f::set_to(GLuint index) const {
	glUniform4fv(index, 1, v);
}


shader_configuration::shader_configuration(const shader_program* program_) : program(program_) { }
shader_configuration::~shader_configuration() { }

void shader_configuration::recreate_opengl_resources() {
}

void shader_configuration::set_uniform(const std::string& name, int v) {
	names.push_back(name);
	setters.push_back(new uniform_setter_1i(v));
}

void shader_configuration::set_uniform(const std::string& name, float v0, float v1, float v2, float v3) {
	names.push_back(name);
	setters.push_back(new uniform_setter_4f(v0, v1, v2, v3));
}

void shader_configuration::use() {
	glUseProgram(program->get_program_id());

	const size_t sz = names.size();
	for (size_t i=0; i<sz; ++i) {
		GLuint location = glGetUniformLocation(program->get_program_id(), names[i].c_str());
		setters[i].set_to(location);
	}
}
