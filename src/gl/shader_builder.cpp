#include <ymse/fileutil.hpp>
#include "shader_builder.hpp"

shader_builder::shader_builder() { }
shader_builder::~shader_builder() { }

void shader_builder::add_shader_from_file(GLenum shader_type, const std::string& filename) {
	shader_spec s;

	s.type = shader_type;
	ymse::read_entire_file(s.source, filename);

	shaders.push_back(s);
}

void shader_builder::bind_attrib_location(GLuint index, const std::string& name) {
	attrib_location_spec s = { index, name };
	attrib_locations.push_back(s);
}
