#ifndef SNYGG_SNAKE_HPP
#define SNYGG_SNAKE_HPP

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <ymse/vec.hpp>
#include "item_with_life.hpp"

class blood_pool;
class item_container;

class snake : public boost::noncopyable, public item_with_life {
	struct impl;
	boost::scoped_ptr<impl> d;

	void forward(float);

public:
	snake(item_container&, float speed, ymse::vec2f pos);
	~snake();

	void score(float amount);

	// 1, 0, -1: left, forward, right
	void set_turn(int dir);
	void move();

	void render(skin&) const;

	bool crashes_with(intersectable_with_circle&) const;

	bool intersect_with_circle(const ymse::vec2f&, float r) const;

	// Start "moving" into a blood pool
	void crack_head();

	void hit_by(player&);

	void assign_death() { die(); }
};

#endif
