#ifndef SNYGG_CRASHABLE_HPP
#define SNYGG_CRASHABLE_HPP

#include "intersectable_with_circle.hpp"
#include "renderable.hpp"

class player;

class crashable : virtual public intersectable_with_circle, virtual public renderable {
public:
	virtual void hit_by(player&) = 0;
	virtual bool is_dead() const = 0;
};

#endif
