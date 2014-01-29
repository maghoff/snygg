#ifndef YMSE_GL_TEXTURE_HPP
#define YMSE_GL_TEXTURE_HPP

namespace ymse {
namespace gl {

class texture {
	unsigned id;

public:
	texture();
	~texture();

	texture(const texture&) = delete;
	texture& operator = (const texture&) = delete;

	texture(texture&&);
	texture& operator = (texture&&);

	unsigned get_id() const;
};

}}

#endif // YMSE_GL_TEXTURE_HPP
