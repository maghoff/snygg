#ifndef SDL_CORE_HPP
#define SDL_CORE_HPP

#include "sdl_main.h"
#include <core.hpp>

struct SDL_Surface;

namespace game {

class sdl_core : public game::core {
	bool inited;
	SDL_Surface* screen;

	game::tick_handler* tick_handler_p;
	game::reshaper* reshaper_p;
	game::keyboard_handler* keyboard_handler_p;
	game::mouse_handler* mouse_handler_p;

	bool running;
	int return_value;

	int desktop_w, desktop_h;
	int windowed_w, windowed_h;
	bool is_fs;

	void set_video_mode_core(int w, int h, bool fullscreen);

public:
	sdl_core();
	~sdl_core();

	void init(int argc, char const * const argv[]) override;
	void set_tick_handler(game::tick_handler*) override;
	void set_reshaper_object(game::reshaper*) override;
	void set_keyboard_handler(game::keyboard_handler*) override;
	void set_mouse_handler(game::mouse_handler*);
	void set_video_mode(int w, int h, bool fullscreen);
	void set_cursor_visible(bool);
	bool get_cursor_visible() const;

	bool is_fullscreen() const;
	void set_fullscreen(bool);
	void toggle_fullscreen();

	int run() override;
	void stop(int);
};

}

#endif

