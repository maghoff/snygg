#include <cassert>
#include <set>
#include <deque>
#include <geometry_intersection.hpp>
#include <rect.hpp>
#include <vec.hpp>
#include "segment_sequence.hpp"


using la::vec2f;


struct segment_sequence::impl {
	std::deque<std::unique_ptr<segment>> body;

	segment& head();
	segment const & head() const;
	segment& tail();
	segment const & tail() const;
};

segment& segment_sequence::impl::head() { return *body.back(); }
segment const & segment_sequence::impl::head() const { return *body.back(); }
segment& segment_sequence::impl::tail() { return *body.front(); }
segment const & segment_sequence::impl::tail() const { return *body.front(); }


segment_sequence::segment_sequence() :
	d(new impl)
{
}

segment_sequence::~segment_sequence() {
}

void segment_sequence::head_forward(float length) {
	d->head().head_forward(length);
}

float segment_sequence::tail_forward(float length) {
	while (!d->body.empty()) {
		length = d->tail().tail_forward(length);
		if (length < 0.f) break;
		d->body.pop_front();
	}
	return length;
}

bool segment_sequence::intersect_with_circle(const la::vec2f& p, float r) const {
	for (auto& s : d->body) {
		if (s->intersect_with_circle(p, r)) return true;
	}
	return false;
}

bool segment_sequence::intersect_with_self(const la::vec2f& p, float r) const {
	float skiplength = r*2.f + 10.f;

	for (auto i = d->body.rbegin(), end = d->body.rend(); i != end; ++i) {
		if ((*i)->intersect_with_circle(p, r, skiplength)) return true;
	}

	if (skiplength <= 0) {
		vec2f tail(get_tail_pos());
		if (circle_with_circle(p, r, tail, 2.5f)) return true;
	}

	return false;
}

vec2f segment_sequence::get_head_pos() const {
	return d->head().get_head_pos();
}

vec2f segment_sequence::get_head_direction() const {
	return d->head().get_head_direction();
}

vec2f segment_sequence::get_tail_pos() const {
	return d->tail().get_tail_pos();
}

vec2f segment_sequence::get_tail_direction() const {
	return d->tail().get_tail_direction();
}

float segment_sequence::length() const {
	double len = 0;
	for (auto& s : d->body) len += s->length();
	return len;
}

void segment_sequence::render(skin& s, float head_b) const {
	render_sequence(d->body, s, head_b);
}

void segment_sequence::push_back(std::unique_ptr<segment>&& s) {
	d->body.emplace_back(std::move(s));
}

void segment_sequence::push_front(std::unique_ptr<segment>&& s) {
	d->body.emplace_front(std::move(s));
}

rectf segment_sequence::bounding_box() const {
	assert(d->body.size() > 0);

	rectf bb = d->head().bounding_box();
	for (auto& s : d->body) bb = ::bounding_box(bb, s->bounding_box());

	return bb;
}

int segment_sequence::left_hline_intersections(la::vec2f p) const {
	int n = 0;
	for (auto& s : d->body) n += s->left_hline_intersections(p);
	return n;
}

void segment_sequence::add_to_triangulator(complex_polygon_triangulator& triangulator) const {
	for (auto& s : d->body) s->add_to_triangulator(triangulator);
}
