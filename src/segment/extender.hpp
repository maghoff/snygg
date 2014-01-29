#ifndef SNYGG_EXTENDER_HPP
#define SNYGG_EXTENDER_HPP

#include <memory>
#include "segment.hpp"

class extender : public segment {
	struct impl;
	std::unique_ptr<impl> d;

public:
	extender(
		ymse::vec2f pos,
		ymse::vec2f dir,
		float length
	);
	~extender();

	void head_forward(float) override;
	float tail_forward(float) override;

	ymse::vec2f get_head_pos() const override;
	ymse::vec2f get_head_direction() const override;
	ymse::vec2f get_tail_pos() const override;
	ymse::vec2f get_tail_direction() const override;

	float length() const override;
	void render(skin&, float head_b) const override;

	ymse::rectf bounding_box() const override;

	int left_hline_intersections(ymse::vec2f p) const override;

	bool intersect_with_circle(const ymse::vec2f&, float r) const override;
	bool intersect_with_circle(const ymse::vec2f&, float r, float& skiplength) const override;
};

#endif // SNYGG_EXTENDER_HPP
