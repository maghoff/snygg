#ifndef SNYGG_SNAKE_HPP
#define SNYGG_SNAKE_HPP

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "renderable.hpp"

class snake : public boost::noncopyable, public renderable {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	snake();
	~snake();

	// 1, 0, -1: left, forward, right
	void set_turn(int dir);
	void forward(float length);

	void render(skin&) const;
};

#endif
