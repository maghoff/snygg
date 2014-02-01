#ifndef YMSE_GAME_HPP
#define YMSE_GAME_HPP

namespace game {

class tick_handler {
	unsigned ticks;

public:
	tick_handler();
	virtual ~tick_handler();

	virtual void render() = 0;

	virtual void tick(unsigned msecs);
	virtual void tick_ms();
	virtual void tick_10ms();
	virtual void tick(); //< Backwards compatible alias for tick_10ms()
};

}

#endif

