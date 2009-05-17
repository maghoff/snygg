#ifndef SNYGG_SNAKE_HPP
#define SNYGG_SNAKE_HPP

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "segment.hpp"

class snake : public boost::noncopyable, public segment {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	snake();
	~snake();

	// 1, 0, -1: left, forward, right
	void set_turn(int dir);
	void forward(float length);

	void render(skin&) const;

	void head_forward(float);
	float tail_forward(float);

	bool intersect_with_circle(float x, float y, float r) const;

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_tail_pos() const;
	ymse::vec2f get_head_direction() const;
};

#endif
