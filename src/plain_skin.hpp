#ifndef SNYGG_PLAIN_SKIN_HPP
#define SNYGG_PLAIN_SKIN_HPP

#include "skin.hpp"

class plain_skin : public skin {
public:
	void fat_arc(float x, float y, float r, float t, float begin, float end);
	void fat_line(float x, float y, float dx, float dy, float len, float t);
};

#endif
