#ifndef SNYGG_SNYGG_HPP
#define SNYGG_SNYGG_HPP

#include <memory>
#include <boost/scoped_ptr.hpp>
#include <ymse/game.hpp>
#include "item_container.hpp"

namespace ymse {
	class sdl_core;
}

class scalable_skin;

class snygg : public ymse::game, public item_container {
	struct impl;
	boost::scoped_ptr<impl> d;

	void set_skin_key(scalable_skin*);

public:
	snygg(const std::string& board_filename);
	~snygg();

	void attach_to_core(ymse::sdl_core&);

	void render();
	void tick();

	void add_item(std::auto_ptr<item>);
	void add_renderable(std::auto_ptr<renderable>);
};

#endif

