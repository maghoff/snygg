#ifndef REVERSE_CONTOUR_SEGMENT_HPP
#define REVERSE_CONTOUR_SEGMENT_HPP

#include "segment_filter.hpp"

class reverse_contour_segment : public segment_filter {
public:
	reverse_contour_segment(segment_ptr);
	reverse_contour_segment(segment*); //< For luabind... hmm...
	~reverse_contour_segment();

	void add_to_triangulator(complex_polygon_triangulator*) const;
};

#endif // REVERSE_CONTOUR_SEGMENT_HPP
