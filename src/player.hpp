#ifndef SNYGG_PLAYER_HPP
#define SNYGG_PLAYER_HPP

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include "renderable.hpp"

namespace ymse {
	class bindable_keyboard_handler;
}

class intersectable_with_circle;

class player : public boost::noncopyable, public renderable {
	class impl;
	boost::scoped_ptr<impl> d;

	void spawn(bool);

public:
	player(ymse::bindable_keyboard_handler&);
	virtual ~player();

	void render(skin&) const;

	void move();
	bool crashes_with(intersectable_with_circle&) const;
	void die();
	void score();
};

#endif
