#ifndef SNYGG_SNYGG_HPP
#define SNYGG_SNYGG_HPP

#include <boost/smart_ptr.hpp>
#include <ymse/game.hpp>

class board;
class skin;
class snake;

class snygg : public ymse::game {
	boost::scoped_ptr<skin> active_skin;
	boost::scoped_ptr<board> active_board;
	boost::scoped_ptr<snake> player;

public:
	snygg();
	~snygg();

	void render();
	void tick();
};

#endif

