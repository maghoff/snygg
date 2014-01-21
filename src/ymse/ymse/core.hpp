#ifndef CORE_HPP
#define CORE_HPP

namespace ymse {

class game;
class reshaper;
class keyboard_handler;
class mouse_handler;

class core {
public:
	core();
	virtual ~core();

	virtual void init(int argc, char const * const argv[]) = 0;

	virtual void set_game_object(game*) = 0;
	virtual void set_reshaper_object(reshaper*) = 0;
	virtual void set_keyboard_handler(keyboard_handler*) = 0;

	virtual int run() = 0;
};

}

#endif

