#ifndef SNYGG_SNAKE_HPP
#define SNYGG_SNAKE_HPP

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/noncopyable.hpp>
#include "renderable.hpp"

class segment;
class skin;

class snake : public boost::noncopyable, public renderable {
	boost::ptr_list<segment> body;
	int dir;

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_tail_pos() const;

public:
	snake();
	~snake();

	// 1, 0, -1: left, forward, right
	void set_turn(int dir);
	void forward(float length);

	void render(skin&) const;
};

#endif
