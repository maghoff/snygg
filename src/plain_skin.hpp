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

	void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end);

	void set_pixels_per_unit(float);
};

#endif
