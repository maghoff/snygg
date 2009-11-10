#ifndef SNYGG_DEAD_PLAYER_HPP
#define SNYGG_DEAD_PLAYER_HPP

#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include "item_with_life.hpp"

class snake;

class dead_player : public boost::noncopyable, public item_with_life {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	dead_player(std::auto_ptr<snake>);
	~dead_player();

	void move();

	void render(skin&) const;

	void hit_by(player&);

	bool intersect_with_circle(float x, float y, float r) const;
};

#endif // SNYGG_DEAD_PLAYER_HPP
