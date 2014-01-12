#ifndef CONTOUR_SEGMENT_HPP
#define CONTOUR_SEGMENT_HPP

#include "segment_filter.hpp"

class contour_segment : public segment_filter {
public:
	contour_segment(segment_ptr);
	contour_segment(segment*); //< For luabind... hmm...
	~contour_segment();

	void add_to_triangulator(complex_polygon_triangulator*) const;
};

#endif // CONTOUR_SEGMENT_HPP
