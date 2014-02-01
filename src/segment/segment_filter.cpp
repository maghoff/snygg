#include <ymse/rect.hpp>
#include <vec.hpp>
#include "segment_filter.hpp"

segment_filter::segment_filter(segment_ptr&& s_) :
	s(std::move(s_))
{
}

segment_filter::~segment_filter() { }

void segment_filter::head_forward(float len) { s->head_forward(len); }
float segment_filter::tail_forward(float len) { return s->tail_forward(len); }

la::vec2f segment_filter::get_head_pos() const { return s->get_head_pos(); }
la::vec2f segment_filter::get_head_direction() const { return s->get_head_direction(); }
la::vec2f segment_filter::get_tail_pos() const { return s->get_tail_pos(); }
la::vec2f segment_filter::get_tail_direction() const { return s->get_tail_direction(); }

float segment_filter::length() const { return s->length(); }

void segment_filter::render(skin& sk, float head_b) const { s->render(sk, head_b); }

ymse::rectf segment_filter::bounding_box() const { return s->bounding_box(); }

int segment_filter::left_hline_intersections(la::vec2f p) const { return s->left_hline_intersections(p); }

void segment_filter::add_to_triangulator(complex_polygon_triangulator& t) const { s->add_to_triangulator(t); }

bool segment_filter::intersect_with_circle(const la::vec2f& p, float r) const { return s->intersect_with_circle(p, r); }
