#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <string>
#include <vector>
#include <GL/glew.h>

#include "opengl_resource.hpp"

struct shader_spec {
	GLenum type;
	std::vector<char> source;
};

struct attrib_location_spec {
	GLuint index;
	std::string name;
};


class shader_builder;

class shader_program : public opengl_resource {
	std::vector<shader_spec> shaders;
	std::vector<attrib_location_spec> attrib_locations;

	GLuint program;

public:
	explicit shader_program(const shader_builder&);
	~shader_program();

	void recreate_opengl_resources();

	GLuint get_program_id() const;

	void set_uniform(const GLchar* name, int);
	void set_uniform(const GLchar* name, float, float, float, float);
};

#endif // SHADER_PROGRAM_HPP
