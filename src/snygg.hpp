#ifndef SNYGG_SNYGG_HPP
#define SNYGG_SNYGG_HPP

#include <boost/scoped_ptr.hpp>
#include <ymse/game.hpp>

namespace ymse {
	class keyboard_handler;
}

class snygg : public ymse::game {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	snygg();
	~snygg();

	void render();
	void tick();

	ymse::keyboard_handler* get_keyboard_handler();
};

#endif

