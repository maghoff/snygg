#ifndef OPEN_SEGMENT_HPP
#define OPEN_SEGMENT_HPP

#include "segment_filter.hpp"

class open_segment : public segment_filter {
public:
	using segment_filter::segment_filter;
	open_segment(segment*); //< For luabind... hmm...
	~open_segment();

	void render(skin&, float head_b) const;

	ymse::rectf bounding_box() const;

	// Always returns 0, since this segment is open (it has only one side)
	int left_hline_intersections(ymse::vec2f p) const;

	bool intersect_with_circle(const ymse::vec2f&, float r) const;

	// Never adds anything
	void add_to_triangulator(complex_polygon_triangulator*) const;
};

#endif

