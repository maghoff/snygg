#ifndef SEGMENT_FILTER_HPP
#define SEGMENT_FILTER_HPP

#include <memory>
#include "segment.hpp"

class segment_filter : public segment {
	segment_ptr s;

public:
	segment_filter(segment_ptr&&);
	~segment_filter();

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

	void add_to_triangulator(complex_polygon_triangulator*) const override;

	bool intersect_with_circle(const ymse::vec2f&, float r) const override;
};

#endif // SEGMENT_FILTER_HPP
