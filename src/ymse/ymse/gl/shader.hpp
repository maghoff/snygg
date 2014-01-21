#ifndef YMSE_GL_SHADER_HPP
#define YMSE_GL_SHADER_HPP

#include <string>
#include <boost/noncopyable.hpp>

namespace ymse {
namespace gl {

class shader : public boost::noncopyable {
	unsigned id;

public:
	shader(unsigned type);
	~shader();

	void source_file(const std::string& filename);

	unsigned get_id() const;
};

}}

#endif // YMSE_GL_SHADER_HPP
