#ifndef SNYGG_SNAKE_HPP
#define SNYGG_SNAKE_HPP

#include <memory>
#include <vec.hpp>
#include "item_with_life.hpp"

class blood_pool;
class item_container;

class snake : public item_with_life {
	struct impl;
	std::unique_ptr<impl> d;

	void forward(float);

public:
	snake(item_container&, float speed, la::vec2f pos);
	~snake();

	snake(const snake&) = delete;
	snake& operator = (const snake&) = delete;

	void score(float amount);

	// 1, 0, -1: left, forward, right
	void set_turn(int dir);
	void move();

	void render(skin&) const;

	bool crashes_with(const intersectable_with_circle&) const;

	bool intersect_with_circle(const la::vec2f&, float r) const;

	// Start "moving" into a blood pool
	void crack_head();

	void hit_by(player&);

	void assign_death() { die(); }
};

#endif
