#ifndef YMSE_SDL_IMG_LOAD_HPP
#define YMSE_SDL_IMG_LOAD_HPP

#include <string>

namespace ymse {
namespace sdl {

class surface;

surface img_load(const std::string& filename);

}}

#endif // YMSE_SDL_IMG_LOAD_HPP
