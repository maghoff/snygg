#ifndef YMSE_GL_TEXTURE_HPP
#define YMSE_GL_TEXTURE_HPP

#include <boost/noncopyable.hpp>

namespace ymse {
namespace gl {

class texture : public boost::noncopyable {
	unsigned id;

public:
	texture();
	~texture();

	unsigned get_id() const;
};

}}

#endif // YMSE_GL_TEXTURE_HPP
