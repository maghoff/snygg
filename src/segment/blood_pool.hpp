#ifndef SNYGG_BLOOD_POOL_HPP
#define SNYGG_BLOOD_POOL_HPP

#include "segment.hpp"

class blood_pool : public segment, public renderable {
	float x, y, r;
public:
	blood_pool(ymse::vec2f pos, float r);
	~blood_pool();

	void head_forward(float) override;
	float tail_forward(float) override;

	bool intersect_with_circle(const ymse::vec2f&, float r) const override;

	ymse::vec2f get_head_pos() const override;
	ymse::vec2f get_head_direction() const override;
	ymse::vec2f get_tail_pos() const override;
	ymse::vec2f get_tail_direction() const override;

	float length() const override;
	void render(skin&, float head_b) const override;

	void render(skin&) const override;

	ymse::rectf bounding_box() const override;

	int left_hline_intersections(ymse::vec2f) const override;
};


#endif
