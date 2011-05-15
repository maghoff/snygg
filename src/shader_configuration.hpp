#ifndef SHADER_CONFIGURATION_HPP
#define SHADER_CONFIGURATION_HPP

#include <boost/ptr_container/ptr_vector.hpp>
#include "opengl_resource.hpp"

struct uniform_setter;
class shader_program;

class shader_configuration : public opengl_resource {
	const shader_program* program;

	std::vector<std::string> names;
	boost::ptr_vector<uniform_setter> setters;

public:
	shader_configuration(const shader_program*);
	~shader_configuration();

	void recreate_opengl_resources();

	void set_uniform(const std::string& name, int);
	void set_uniform(const std::string& name, float, float, float, float);

	void use();
};

#endif // SHADER_CONFIGURATION_HPP
