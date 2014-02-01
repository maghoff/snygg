#ifndef GL_RAII_TEXTURE_HPP
#define GL_RAII_TEXTURE_HPP

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

}

#endif // GL_RAII_TEXTURE_HPP
