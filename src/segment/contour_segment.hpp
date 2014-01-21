#ifndef CONTOUR_SEGMENT_HPP
#define CONTOUR_SEGMENT_HPP

#include "segment_filter.hpp"

class contour_segment : public segment_filter {
public:
	using segment_filter::segment_filter;
	contour_segment(segment*); //< For luabind... hmm...
	~contour_segment();

	void add_to_triangulator(complex_polygon_triangulator*) const override;
};

#endif // CONTOUR_SEGMENT_HPP
