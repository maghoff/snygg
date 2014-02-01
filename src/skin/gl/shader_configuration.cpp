#include <vector>
#include <ymse/gl.h>
#include <ymse/gl/texture.hpp>
#include <load_jpeg.hpp>
#include <surface.hpp>
#include <copy.hpp>
#include "shader_configuration.hpp"
#include "shader_program.hpp"


struct uniform_setter {
	// All the subclasses have trivial destructors

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


struct shader_configuration::impl {
	const shader_program* program;

	std::vector<std::string> names;
	std::vector<std::unique_ptr<uniform_setter>> setters;

	std::vector<image::surface> surfaces;
	std::vector<ymse::gl::texture> textures;
};


shader_configuration::shader_configuration(const shader_program* program) :
	d(new impl)
{
	d->program = program;
}

shader_configuration::~shader_configuration() { }

void shader_configuration::recreate_opengl_resources() {
	for (unsigned i = 0; i < d->surfaces.size(); ++i) {
		image::gl::copy_surface_to_texture(d->surfaces[i], d->textures[i].get_id());
	}
}

void shader_configuration::set_uniform(const std::string& name, int v) {
	d->names.push_back(name);
	d->setters.emplace_back(new uniform_setter_1i(v));
}

void shader_configuration::set_uniform(const std::string& name, float v0, float v1, float v2, float v3) {
	d->names.push_back(name);
	d->setters.emplace_back(new uniform_setter_4f(v0, v1, v2, v3));
}

void shader_configuration::add_texture(const std::string& name, const std::string& filename) {
	set_uniform(name, d->textures.size());
	d->surfaces.emplace_back(image::load_jpeg(filename));
	d->textures.resize(d->textures.size() + 1);
}

void shader_configuration::use() const {
	glUseProgram(d->program->get_program_id());

	const size_t sz = d->names.size();
	for (size_t i=0; i<sz; ++i) {
		GLuint location = glGetUniformLocation(d->program->get_program_id(), d->names[i].c_str());
		d->setters[i]->set_to(location);
	}

	for (unsigned i = 0; i < d->textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, d->textures[i].get_id());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
}
