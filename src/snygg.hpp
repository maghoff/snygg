#ifndef SNYGG_SNYGG_HPP
#define SNYGG_SNYGG_HPP

#include <boost/smart_ptr.hpp>
#include <ymse/game.hpp>

class skin;

class snygg : public ymse::game {
	boost::shared_ptr<skin> active_skin;

public:
	snygg();
	~snygg();

	void render();
	void tick();
};

#endif

