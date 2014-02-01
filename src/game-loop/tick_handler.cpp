#include "tick_handler.hpp"

namespace game {

tick_handler::tick_handler() :
	ticks(0)
{
}

tick_handler::~tick_handler() {
}

void tick_handler::tick(unsigned msecs) {
	// If we are more than five seconds behind, something has happened.
	// Examples are computer sleep and debugger break.
	if (msecs > 5000) return;

	for (unsigned i=0; i<msecs; ++i) {
		tick_ms();
		if ((ticks + i) % 10 == 0) tick_10ms();
	}
	ticks += msecs;
}

void tick_handler::tick_ms() { }
void tick_handler::tick_10ms() { tick(); }
void tick_handler::tick() { }

}

