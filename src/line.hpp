#ifndef SNYGG_LINE_HPP
#define SNYGG_LINE_HPP

#include "segment.hpp"

class line : public segment {
	float x, y;
	float dx, dy;
	float length;
	float thickness;

public:
	line(
		float x, float y,
		float dx, float dy,
		float length
	);
	line(
		ymse::vec2f pos,
		ymse::vec2f dir,
		float length
	);
	~line();

	void head_forward(float);
	float tail_forward(float);

	bool intersect_with_circle(float x, float y, float r) const;

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_tail_pos() const;
	ymse::vec2f get_head_direction() const;

	void render(skin&) const;
};

#endif
