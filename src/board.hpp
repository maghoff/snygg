#ifndef SNYGG_BOARD_HPP
#define SNYGG_BOARD_HPP

#include <boost/scoped_ptr.hpp>
#include "renderable.hpp"

class board : public renderable {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	board();
	~board();

	void render(skin&) const;
};

#endif
