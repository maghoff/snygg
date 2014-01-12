#include <cmath>
#include <ymse/geometry_intersection.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "../skin.hpp"
#include "open_segment.hpp"

open_segment::open_segment(segment_ptr s_) :
	segment_filter(s_)
{
}

open_segment::open_segment(segment* s_) :
	segment_filter(segment_ptr(s_))
{
}

open_segment::~open_segment() {
}

void open_segment::render(skin& sk, float head_b) const {
	using ymse::vec2f;

	vec2f head(get_head_pos()), head_dir(get_head_direction());
	sk.cap(head, atan2(head_dir[1], head_dir[0]), M_PI * 0.5, head_b);

	segment_filter::render(sk, head_b);

	vec2f tail(get_tail_pos()), tail_dir(get_tail_direction());
	sk.cap(tail, atan2(tail_dir[1], tail_dir[0]), M_PI * 1.5, head_b + length());
}

ymse::rectf open_segment::bounding_box() const {
	// Should consider head- and tail-caps
	return segment_filter::bounding_box();
}

int open_segment::left_hline_intersections(ymse::vec2f p) const {
	return 0;
}

bool open_segment::intersect_with_circle(const ymse::vec2f& p, float r) const {
	namespace i = ymse::intersect;
	using ymse::vec2f;

	vec2f head(get_head_pos());
	vec2f tail(get_tail_pos());

	return
		i::circle_with_circle(p, r, head, 2.5f) ||
		i::circle_with_circle(p, r, tail, 2.5f) ||
		segment_filter::intersect_with_circle(p, r)
	;
}

void open_segment::add_to_triangulator(complex_polygon_triangulator*) const {
}
