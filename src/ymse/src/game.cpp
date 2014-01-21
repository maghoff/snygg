#include "game.hpp"

namespace ymse {

game::game() :
	ticks(0)
{
}

game::~game() {
}

void game::tick(unsigned msecs) {
	// If we are more than five seconds behind, something has happened.
	// Examples are computer sleep and debugger break.
	if (msecs > 5000) return;

	for (unsigned i=0; i<msecs; ++i) {
		tick_ms();
		if ((ticks + i) % 10 == 0) tick_10ms();
	}
	ticks += msecs;
}

void game::tick_ms() { }
void game::tick_10ms() { tick(); }
void game::tick() { }

}

