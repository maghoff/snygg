#ifndef YMSE_SDL_CORE_HPP
#define YMSE_SDL_CORE_HPP

#include "sdl_main.h"
#include "core.hpp"

struct SDL_Surface;

namespace ymse {

class sdl_core : public core {
	bool inited;
	SDL_Surface* screen;

	game* game_p;
	reshaper* reshaper_p;
	keyboard_handler* keyboard_handler_p;
	mouse_handler* mouse_handler_p;

	bool running;
	int return_value;

	int desktop_w, desktop_h;
	int windowed_w, windowed_h;
	bool is_fs;

	void set_video_mode_core(int w, int h, bool fullscreen);

public:
	sdl_core();
	~sdl_core();

	void init(int argc, char const * const argv[]);
	void set_game_object(game*);
	void set_reshaper_object(reshaper*);
	void set_keyboard_handler(keyboard_handler*);
	void set_mouse_handler(mouse_handler*);
	void set_video_mode(int w, int h, bool fullscreen);
	void set_cursor_visible(bool);
	bool get_cursor_visible() const;

	bool is_fullscreen() const;
	void set_fullscreen(bool);
	void toggle_fullscreen();

	int run();
	void stop(int);
};

}

#endif

