#ifndef SNYGG_BLOOD_POOL_HPP
#define SNYGG_BLOOD_POOL_HPP

#include "segment.hpp"

class blood_pool : public segment, public renderable {
	float x, y, r;
public:
	blood_pool(la::vec2f pos, float r);
	~blood_pool();

	void head_forward(float) override;
	float tail_forward(float) override;

	bool intersect_with_circle(const la::vec2f&, float r) const override;

	la::vec2f get_head_pos() const override;
	la::vec2f get_head_direction() const override;
	la::vec2f get_tail_pos() const override;
	la::vec2f get_tail_direction() const override;

	float length() const override;
	void render(skin&, float head_b) const override;

	void render(skin&) const override;

	rectf bounding_box() const override;

	int left_hline_intersections(la::vec2f) const override;
};


#endif
