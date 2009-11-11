#ifndef SNYGG_SNYGG_HPP
#define SNYGG_SNYGG_HPP

#include <memory>
#include <boost/scoped_ptr.hpp>
#include <ymse/game.hpp>
#include "item_container.hpp"

namespace ymse {
	class keyboard_handler;
	class reshaper;
}

class snygg : public ymse::game, public item_container {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	snygg();
	~snygg();

	void render();
	void tick();

	ymse::keyboard_handler* get_keyboard_handler();
	ymse::reshaper* get_reshaper_object();

	void add_item(std::auto_ptr<item>);
	void add_renderable(std::auto_ptr<renderable>);
};

#endif

