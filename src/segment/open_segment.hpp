#ifndef OPEN_SEGMENT_HPP
#define OPEN_SEGMENT_HPP

#include "segment_filter.hpp"

class open_segment : public segment_filter {
public:
	using segment_filter::segment_filter;
	~open_segment();

	void render(skin&, float head_b) const override;

	rectf bounding_box() const override;

	// Always returns 0, since this segment is open (it has only one side)
	int left_hline_intersections(la::vec2f p) const override;

	bool intersect_with_circle(const la::vec2f&, float r) const override;

	// Never adds anything
	void add_to_triangulator(complex_polygon_triangulator&) const override;
};

#endif

