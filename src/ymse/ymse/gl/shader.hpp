#ifndef YMSE_GL_SHADER_HPP
#define YMSE_GL_SHADER_HPP

#include <string>

namespace ymse {
namespace gl {

class shader {
	unsigned id;

public:
	shader(unsigned type);
	~shader();

	shader(const shader&) = delete;
	shader& operator = (const shader&) = delete;

	void source_file(const std::string& filename);

	unsigned get_id() const;
};

}}

#endif // YMSE_GL_SHADER_HPP
