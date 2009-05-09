#ifndef SCORCHED_HPP
#define SCORCHED_HPP

#include <ymse/game.hpp>

class snygg : public ymse::game {
public:
	snygg();
	~snygg();

	void render();
	void tick();
};

#endif

