#include <stdexcept>
#include <string>
#include <SDL_image.h>
#include "sdl/surface.hpp"
//#include "img_load.hpp"

namespace ymse {
namespace sdl {

surface img_load(const std::string& filename) {
	surface s(IMG_Load(filename.c_str()));
	if (!s.get()) {
		throw std::runtime_error("When loading \"" + filename + "\": " + IMG_GetError());
	}
	return s;
}

}}
