#ifndef SNYGG_PLAIN_SKIN_HPP
#define SNYGG_PLAIN_SKIN_HPP

#include "scalable_skin.hpp"

class plain_skin : public scalable_skin {
public:
	plain_skin();

	void circle(ymse::vec2f p, float r);

	void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end);
};

#endif
