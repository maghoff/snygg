#ifndef SNYGG_SNYGG_HPP
#define SNYGG_SNYGG_HPP

#include <boost/smart_ptr.hpp>
#include <ymse/game.hpp>

namespace ymse {
	class keyboard_handler;
	class bindable_keyboard_handler;
	class opposite_keys;
}

class board;
class skin;
class snake;

class snygg : public ymse::game {
	boost::scoped_ptr<skin> active_skin;
	boost::scoped_ptr<board> active_board;
	boost::scoped_ptr<snake> player;

	boost::scoped_ptr<ymse::bindable_keyboard_handler> kbd;
	boost::scoped_ptr<ymse::opposite_keys> dir;

public:
	snygg();
	~snygg();

	void render();
	void tick();

	ymse::keyboard_handler* get_keyboard_handler();
};

#endif

