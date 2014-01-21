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

	void head_forward(float) override;
	float tail_forward(float) override;

	bool intersect_with_circle(const ymse::vec2f&, float r) const override;
	bool intersect_with_circle(const ymse::vec2f&, float r, float& skiplength) const override;

	ymse::vec2f get_head_pos() const override;
	ymse::vec2f get_head_direction() const override;
	ymse::vec2f get_tail_pos() const override;
	ymse::vec2f get_tail_direction() const override;

	float length() const override;
	void render(skin&, float head_b) const override;

	ymse::rectf bounding_box() const override;

	int left_hline_intersections(ymse::vec2f) const override;
};

#endif
