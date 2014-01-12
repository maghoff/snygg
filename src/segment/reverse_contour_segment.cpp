#include "../complex_polygon_triangulator.hpp"
#include "reverse_contour_segment.hpp"

reverse_contour_segment::reverse_contour_segment(segment_ptr s) :
	segment_filter(s)
{
}

reverse_contour_segment::reverse_contour_segment(segment* s) :
	segment_filter(segment_ptr(s))
{
}

reverse_contour_segment::~reverse_contour_segment() { }

void reverse_contour_segment::add_to_triangulator(complex_polygon_triangulator* triangulator) const {
	triangulator->start_reverse_contour();
	segment_filter::add_to_triangulator(triangulator);
	triangulator->end_contour();
}
