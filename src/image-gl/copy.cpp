#include <GL/glew.h>
#include <surface.hpp>

namespace image {
namespace gl {

void copy_surface_to_texture(const surface& s, unsigned int gl_texture) {
	GLuint format = s.components() == 4 ? GL_RGBA : GL_RGB;

	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s.width(), s.height(), 0, format, GL_UNSIGNED_BYTE, s.data().data());
	glGenerateMipmap(GL_TEXTURE_2D);
}

}}
