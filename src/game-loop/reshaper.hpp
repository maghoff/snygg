#ifndef GAME_LOOP_RESHAPER_HPP
#define GAME_LOOP_RESHAPER_HPP

namespace game {

class reshaper {
public:
	reshaper();
	virtual ~reshaper();

	virtual void reshape(int width, int height) = 0;
};

}

#endif

