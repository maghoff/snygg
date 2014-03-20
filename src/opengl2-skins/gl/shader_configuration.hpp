#ifndef SHADER_CONFIGURATION_HPP
#define SHADER_CONFIGURATION_HPP

#include <memory>
#include <matrix.hpp>
#include "opengl_resource.hpp"

struct uniform_setter;
class shader_program;

class shader_configuration : public opengl_resource {
	struct impl;
	std::unique_ptr<impl> d;

public:
	shader_configuration(const shader_program*);
	~shader_configuration() override;

	void recreate_opengl_resources() override;

	void set_uniform(const std::string& name, int);
	void set_uniform(const std::string& name, float, float, float);
	void set_uniform(const std::string& name, float, float, float, float);

	void set_uniform(const std::string& name, const la::vec4f&);

	void set_uniform(const std::string& name, const la::matrix33f&);

	void add_texture(const std::string& sampler_name, const std::string& filename);

	void use() const;
};

#endif // SHADER_CONFIGURATION_HPP
