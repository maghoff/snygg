#ifndef SNYGG_EXTENDER_HPP
#define SNYGG_EXTENDER_HPP

#include <memory>
#include "segment.hpp"

class extender : public segment {
	struct impl;
	std::unique_ptr<impl> d;

public:
	extender(
		la::vec2f pos,
		la::vec2f dir,
		float length
	);
	~extender();

	void head_forward(float) override;
	float tail_forward(float) override;

	la::vec2f get_head_pos() const override;
	la::vec2f get_head_direction() const override;
	la::vec2f get_tail_pos() const override;
	la::vec2f get_tail_direction() const override;

	float length() const override;
	void render(skin&, float head_b) const override;

	rectf bounding_box() const override;

	int left_hline_intersections(la::vec2f p) const override;

	bool intersect_with_circle(const la::vec2f&, float r) const override;
	bool intersect_with_circle(const la::vec2f&, float r, float& skiplength) const override;
};

#endif // SNYGG_EXTENDER_HPP
