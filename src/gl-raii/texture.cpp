#include <algorithm>
#include <GL/glew.h>
#include "texture.hpp"

namespace gl {

texture::texture() {
	glGenTextures(1, &id);
}

texture::~texture() {
	glDeleteTextures(1, &id);
}

texture::texture(texture&& rhs) {
	std::swap(id, rhs.id);
}

texture& texture::operator = (texture&& rhs) {
	std::swap(id, rhs.id);
	return *this;
}

unsigned texture::get_id() const {
	return id;
}

}
