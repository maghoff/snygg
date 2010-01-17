#ifndef SNYGG_ARC_HPP
#define SNYGG_ARC_HPP

#include "segment.hpp"

class arc : public segment {
	float x, y, r;
	float begin, end;
	float direction;
	float thickness;

public:
	arc(
		float x, float y,
		float r,
		float begin, float end,
		float direction
	);
	arc(
		ymse::vec2f pos,
		float r,
		float begin, float end,
		float direction
	);
	~arc();

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
