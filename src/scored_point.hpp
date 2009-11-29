#ifndef SNYGG_SCORED_POINT_HPP
#define SNYGG_SCORED_POINT_HPP

#include "segment.hpp"

class scored_point : public segment {
	float x, y, r, min_r, dx, dy;

public:
	scored_point(
		ymse::vec2f pos, float r,
		float min_r,
		ymse::vec2f dir
	);
	~scored_point();

	void head_forward(float);
	float tail_forward(float);

	bool intersect_with_circle(const ymse::vec2f&, float r) const;

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_tail_pos() const;
	ymse::vec2f get_head_direction() const;

	float length() const;
	void render(skin&, float head_b) const;
};

#endif
