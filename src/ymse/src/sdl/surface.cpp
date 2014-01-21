#include "gl.h"
#include "glu.h"
#include <SDL.h>
#include "gl/texture.hpp"
#include "sdl/surface.hpp"

namespace ymse {
namespace sdl {

surface::surface() : s(0) { }
surface::surface(SDL_Surface* s_) : s(s_) { }
surface::surface(const surface& rhs) { operator = (rhs); }

surface::~surface() {
	// SDL_FreeSurface decrements refcount
	SDL_FreeSurface(s);
}

surface& surface::operator = (const surface& rhs) {
	s = rhs.s;
	s->refcount++;
	return *this;
}

void surface::copy_to(gl::texture& t) const {
	SDL_PixelFormat desiredFormat;
	desiredFormat.BitsPerPixel = 32;
	desiredFormat.BytesPerPixel = 4;
	desiredFormat.Amask = 0xFF000000;
	desiredFormat.Bmask = 0x00FF0000;
	desiredFormat.Gmask = 0x0000FF00;
	desiredFormat.Rmask = 0x000000FF;
	desiredFormat.Ashift = 24;
	desiredFormat.Bshift = 16;
	desiredFormat.Gshift =  8;
	desiredFormat.Rshift =  0;
	desiredFormat.Aloss = 0;
	desiredFormat.Rloss = 0;
	desiredFormat.Gloss = 0;
	desiredFormat.Bloss = 0;
	desiredFormat.alpha = 0xFF;
	desiredFormat.colorkey = 0;
	desiredFormat.palette = 0;

	surface transformed(SDL_ConvertSurface(s, &desiredFormat, SDL_SWSURFACE));

	glBindTexture(GL_TEXTURE_2D, t.get_id());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, transformed->w, transformed->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, transformed->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
}

}}
