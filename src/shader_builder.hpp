#ifndef SHADER_BUILDER_HPP
#define SHADER_BUILDER_HPP

#include <string>
#include "shader_program.hpp"

class shader_builder {
public:
	std::vector<shader_spec> shaders;
	std::vector<attrib_location_spec> attrib_locations;

	shader_builder();
	~shader_builder();

	void add_shader_from_file(GLenum shader_type, const std::string& filename);
	void bind_attrib_location(GLuint index, const std::string& name);
};

#endif // SHADER_BUILDER_HPP
