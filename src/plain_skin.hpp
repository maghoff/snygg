#ifndef SNYGG_PLAIN_SKIN_HPP
#define SNYGG_PLAIN_SKIN_HPP

#include <ymse/pixels_per_unit_listener.hpp>
#include "skin.hpp"

class plain_skin : public skin, public ymse::pixels_per_unit_listener {
	float detail_modifier;

	float get_step_size(float r);

public:
	plain_skin();

	void circle(ymse::vec2f p, float r);

	void fat_arc(float x, float y, float r, float t, float begin, float end);
	void fat_line(float x, float y, float dx, float dy, float len, float t);

	void set_pixels_per_unit(float);
};

#endif
