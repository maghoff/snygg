#ifndef SNYGG_SNAKE_HPP
#define SNYGG_SNAKE_HPP

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "item_with_life.hpp"

class blood_pool;

class snake : public boost::noncopyable, public item_with_life {
	struct impl;
	boost::scoped_ptr<impl> d;

	void forward(float);

public:
	snake(float speed);
	~snake();

	void score(float amount);

	// 1, 0, -1: left, forward, right
	void set_turn(int dir);
	void move();

	void render(skin&) const;

	bool crashes_with(intersectable_with_circle&) const;

	bool intersect_with_circle(float x, float y, float r) const;

	// Start "moving" into a blood pool
	blood_pool* crack_head();

	// To check if the blood pool is the only remaining segment
	bool is_single_segment() const;

	void hit_by(player&);

	void assign_death() { die(); }
};

#endif
