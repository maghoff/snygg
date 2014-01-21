#ifndef YMSE_GAME_HPP
#define YMSE_GAME_HPP

namespace ymse {

class game {
	unsigned ticks;

public:
	game();
	virtual ~game();

	virtual void render() = 0;

	virtual void tick(unsigned msecs);
	virtual void tick_ms();
	virtual void tick_10ms();
	virtual void tick(); //< Backwards compatible alias for tick_10ms()
};

}

#endif

