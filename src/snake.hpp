#ifndef SNYGG_SNAKE_HPP
#define SNYGG_SNAKE_HPP

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/noncopyable.hpp>
#include "renderable.hpp"

class segment;
class skin;

class snake : public boost::noncopyable, public renderable {
	boost::ptr_list<segment> body;

public:
	snake();
	~snake();

	void render(skin&) const;
};

#endif
