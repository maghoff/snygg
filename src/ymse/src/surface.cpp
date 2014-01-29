#include <unistd.h>
#include <execinfo.h>
#include <iostream>
#include "gl.h"
#include "gl/texture.hpp"
#include "surface.hpp"

namespace ymse {

surface::surface(const std::vector<unsigned char>& data_, unsigned int width_, unsigned int height_, unsigned int components_) :
	data(data_),
	width(width_),
	height(height_),
	components(components_)
{}

surface::surface(std::vector<unsigned char>&& data_, unsigned int width_, unsigned int height_, unsigned int components_) :
	data(data_),
	width(width_),
	height(height_),
	components(components_)
{}

void surface::copy_to(gl::texture& t) const {
	GLuint format = components == 4 ? GL_RGBA : GL_RGB;

	glBindTexture(GL_TEXTURE_2D, t.get_id());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data.data());
	glGenerateMipmap(GL_TEXTURE_2D);
}

}
