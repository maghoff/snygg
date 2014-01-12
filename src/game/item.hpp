#ifndef SNYGG_ITEM_HPP
#define SNYGG_ITEM_HPP

#include "../core/intersectable_with_circle.hpp"
#include "../core/renderable.hpp"

class player;

class item :
	public intersectable_with_circle,
	public renderable
{
public:
	~item();

	virtual void move() = 0;
	virtual void hit_by(player&) = 0;
	virtual bool is_dead() const = 0;
};

#endif

