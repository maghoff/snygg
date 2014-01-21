#include "gl.h"
#include "gl/texture.hpp"

namespace ymse {
namespace gl {

texture::texture() {
	glGenTextures(1, &id);
}

texture::~texture() {
	glDeleteTextures(1, &id);
}

unsigned texture::get_id() const {
	return id;
}

}}
