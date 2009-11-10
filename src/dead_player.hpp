#ifndef SNYGG_DEAD_PLAYER_HPP
#define SNYGG_DEAD_PLAYER_HPP

#include <memory>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include "renderable.hpp"

class snake;

class dead_player : public boost::noncopyable, public renderable {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	dead_player(std::auto_ptr<snake>);
	~dead_player();

	void move();

	void render(skin&) const;
};

#endif // SNYGG_DEAD_PLAYER_HPP
