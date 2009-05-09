#ifndef SNYGG_BOARD_HPP
#define SNYGG_BOARD_HPP

#include <boost/ptr_container/ptr_vector.hpp>
#include "renderable.hpp"

class segment;

class board : public renderable {
	boost::ptr_vector<segment> b;

public:
	board();
	~board();

	void render(skin&) const;
};

#endif
