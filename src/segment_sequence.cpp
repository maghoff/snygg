#include <set>
#include <boost/ptr_container/ptr_list.hpp>
#include <ymse/geometry_intersection.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "segment_sequence.hpp"


using ymse::vec2f;


struct segment_sequence::impl {
	boost::ptr_list<segment> body;

	segment& head();
	segment const & head() const;
	segment& tail();
	segment const & tail() const;
};

segment& segment_sequence::impl::head() { return body.back(); }
segment const & segment_sequence::impl::head() const { return body.back(); }
segment& segment_sequence::impl::tail() { return body.front(); }
segment const & segment_sequence::impl::tail() const { return body.front(); }


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

bool segment_sequence::intersect_with_circle(const ymse::vec2f& p, float r) const {
	typedef boost::ptr_list<segment>::iterator iter;
	iter end = d->body.end();
	for (iter i = d->body.begin(); i !=  end; ++i) {
		if (i->intersect_with_circle(p, r)) return true;
	}
	return false;
}

bool segment_sequence::intersect_with_self(const ymse::vec2f& p, float r) const {
	typedef boost::ptr_list<segment>::reverse_iterator iter;
	iter end = d->body.rend();

	float skiplength = r*2.f + 10.f;

	for (iter i = d->body.rbegin(); i != end; ++i) {
		if (i->intersect_with_circle(p, r, skiplength)) return true;
	}

	if (skiplength <= 0) {
		namespace i = ymse::intersect;
		vec2f tail(get_tail_pos());
		if (i::circle_with_circle(p, r, tail, 2.5f)) return true;
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

	typedef boost::ptr_list<segment>::const_iterator iter;
	iter end = d->body.end();
	for (iter i = d->body.begin(); i != end; ++i) {
		len += i->length();
	}

	return len;
}

void segment_sequence::render(skin& s, float head_b) const {
	render_sequence(d->body, s, head_b);
}

void segment_sequence::push_back(std::auto_ptr<segment> s) {
	d->body.push_back(s);
}

void segment_sequence::push_front(std::auto_ptr<segment> s) {
	d->body.push_front(s);
}

void segment_sequence::push_back(segment* s) {
	d->body.push_back(s);
}

ymse::rectf segment_sequence::bounding_box() const {
	assert(d->body.size() > 0);

	ymse::rectf bb = d->body.front().bounding_box();

	typedef boost::ptr_list<segment>::const_iterator iter;
	iter end = d->body.end();
	for (iter i = d->body.begin(); i != end; ++i) {
		bb = ymse::bounding_box(bb, i->bounding_box());
	}

	return bb;
}

int segment_sequence::left_hline_intersections(ymse::vec2f p) const {
	int n = 0;

	typedef boost::ptr_list<segment>::const_iterator iter;
	iter end = d->body.end();
	for (iter i = d->body.begin(); i != end; ++i) {
		n += i->left_hline_intersections(p);
	}

	return n;
}

void segment_sequence::add_to_triangulator(complex_polygon_triangulator* triangulator) const {
	typedef boost::ptr_list<segment>::const_iterator iter;
	iter end = d->body.end();
	for (iter i = d->body.begin(); i != end; ++i) {
		i->add_to_triangulator(triangulator);
	}
}
