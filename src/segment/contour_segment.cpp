#include "../board/complex_polygon_triangulator.hpp"
#include "contour_segment.hpp"

contour_segment::contour_segment(segment_ptr s) :
	segment_filter(s)
{
}

contour_segment::contour_segment(segment* s) :
	segment_filter(segment_ptr(s))
{
}

contour_segment::~contour_segment() { }

void contour_segment::add_to_triangulator(complex_polygon_triangulator* triangulator) const {
	triangulator->start_contour();
	segment_filter::add_to_triangulator(triangulator);
	triangulator->end_contour();
}
