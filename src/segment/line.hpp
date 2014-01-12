#ifndef SNYGG_LINE_HPP
#define SNYGG_LINE_HPP

#include "segment.hpp"

class line : public segment {
	float x, y;
	float dx, dy;
	float len;
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

	bool intersect_with_circle(const ymse::vec2f&, float r) const;
	bool intersect_with_circle(const ymse::vec2f&, float r, float& skiplength) const;

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_head_direction() const;
	ymse::vec2f get_tail_pos() const;
	ymse::vec2f get_tail_direction() const;

	float length() const;
	void render(skin&, float head_b) const;

	ymse::rectf bounding_box() const;

	int left_hline_intersections(ymse::vec2f) const;
};

#endif
