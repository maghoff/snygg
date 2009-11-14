#ifndef SNYGG_BOARD_HPP
#define SNYGG_BOARD_HPP

#include <boost/scoped_ptr.hpp>
#include "intersectable_with_circle.hpp"
#include "renderable.hpp"

class board : public renderable, public intersectable_with_circle {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	board();
	~board();

	void render(skin&) const;

	bool intersect_with_circle(const ymse::vec2f&, float r) const;
};

#endif
