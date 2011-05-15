#ifndef SHADER_CONFIGURATION_HPP
#define SHADER_CONFIGURATION_HPP

#include <boost/scoped_ptr.hpp>
#include "opengl_resource.hpp"

struct uniform_setter;
class shader_program;

class shader_configuration : public opengl_resource {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	shader_configuration(const shader_program*);
	~shader_configuration();

	void recreate_opengl_resources();

	void set_uniform(const std::string& name, int);
	void set_uniform(const std::string& name, float, float, float, float);

	void add_texture(const std::string& sampler_name, const std::string& filename);

	void use();
};

#endif // SHADER_CONFIGURATION_HPP
