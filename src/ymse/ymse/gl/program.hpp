#ifndef YMSE_GL_PROGRAM_HPP
#define YMSE_GL_PROGRAM_HPP

#include <string>

namespace ymse {
namespace gl {

class shader;

class program {
	unsigned id;

public:
	program();
	~program();

	program(const program&) = delete;
	program& operator = (const program&) = delete;

	void attach(const shader&);

	void bind_attrib_location(unsigned index, const char* name);

	void link();

	template <typename T>
	void set_uniform(const std::string& name, T);

	template <typename T>
	void set_uniform(const std::string& name, T, T, T, T);

	unsigned get_id();
};

}}

#endif // YMSE_GL_PROGRAM_HPP
