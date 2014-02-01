#ifndef GL_RAII_SHADER_HPP
#define GL_RAII_SHADER_HPP

#include <string>

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

}

#endif // GL_RAII_SHADER_HPP
