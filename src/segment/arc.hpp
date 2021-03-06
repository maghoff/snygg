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
		la::vec2f pos,
		float r,
		float begin, float end,
		float direction
	);
	~arc();

	void head_forward(float) override;
	float tail_forward(float) override;

	bool intersect_with_circle(const la::vec2f&, float r) const override;
	bool intersect_with_circle(const la::vec2f&, float r, float& skiplength) const override;

	la::vec2f get_head_pos() const override;
	la::vec2f get_head_direction() const override;
	la::vec2f get_tail_pos() const override;
	la::vec2f get_tail_direction() const override;

	float length() const override;
	void render(skin&, float head_b) const override;

	rectf bounding_box() const override;

	int left_hline_intersections(la::vec2f) const override;

	void add_to_triangulator(complex_polygon_triangulator&) const override;
};

#endif
