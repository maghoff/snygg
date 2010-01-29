#include <cmath>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "skin.hpp"
#include "open_segment.hpp"

open_segment::open_segment(segment_ptr s_) :
	s(s_)
{
}

open_segment::open_segment(segment* s_) :
	s(s_)
{
}

open_segment::~open_segment() {
}

void open_segment::head_forward(float l) { s->head_forward(l); }
float open_segment::tail_forward(float l) { return s->tail_forward(l); }

ymse::vec2f open_segment::get_head_pos() const { return s->get_head_pos(); }
ymse::vec2f open_segment::get_head_direction() const { return s->get_head_direction(); }
ymse::vec2f open_segment::get_tail_pos() const { return s->get_tail_pos(); }
ymse::vec2f open_segment::get_tail_direction() const { return s->get_tail_direction(); }

float open_segment::length() const { return s->length(); }

void open_segment::render(skin& sk, float head_b) const {
	using ymse::vec2f;

	vec2f head(s->get_head_pos()), head_dir(s->get_head_direction());
	sk.cap(head, atan2(head_dir[1], head_dir[0]), M_PI * 0.5, head_b);

	s->render(sk, head_b);

	vec2f tail(s->get_tail_pos()), tail_dir(s->get_tail_direction());
	sk.cap(tail, atan2(tail_dir[1], tail_dir[0]), M_PI * 1.5, head_b + s->length());
}

ymse::rectf open_segment::bounding_box() const {
	// Should consider head- and tail-caps
	return s->bounding_box();
}

int open_segment::left_hline_intersections(ymse::vec2f p) const {
	return 0;
}

bool open_segment::intersect_with_circle(const ymse::vec2f& p, float r) const {
	// Should consider head- and tail-caps
	return s->intersect_with_circle(p, r);
}

bool open_segment::intersect_with_circle(const ymse::vec2f& p, float r, float& skiplength) const {
	// Should consider head- and tail-caps
	return s->intersect_with_circle(p, r, skiplength);
}

