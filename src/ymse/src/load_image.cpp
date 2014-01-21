#include "gl.h"
#include "glu.h"
#include <string>
#include <SDL.h>
#include "sdl/img_load.hpp"
#include "sdl/surface.hpp"
//#include "load_image.hpp"

namespace ymse {

GLuint load_texture(const std::string& file) {
	sdl::surface surface = sdl::img_load(file);

	GLuint texture;
	glGenTextures(1, &texture);

	SDL_PixelFormat *format = surface->format;

	int bpp = format->Amask ? 4 : 3;
	int gl_format = format->Amask ? GL_RGBA : GL_RGB;

	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, surface->w, surface->h, gl_format, GL_UNSIGNED_BYTE, surface->pixels);

	return texture;
}

}
