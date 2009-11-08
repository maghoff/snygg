#ifndef SNYGG_ITEM_HPP
#define SNYGG_ITEM_HPP

#include "intersectable_with_circle.hpp"
#include "renderable.hpp"

class player;

class item :
	public intersectable_with_circle,
	public renderable
{
public:
	~item();

	virtual void hit_by(player&) = 0;
	virtual bool is_dead() = 0;
};

#endif

