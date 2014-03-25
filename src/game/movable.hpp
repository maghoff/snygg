#ifndef SNYGG_MOVABLE_HPP
#define SNYGG_MOVABLE_HPP

#include "crashable.hpp"

class movable : virtual public crashable {
public:
	virtual ~movable();

	virtual void move() = 0;
};

#endif
