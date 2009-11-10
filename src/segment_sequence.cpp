#include <boost/ptr_container/ptr_list.hpp>
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

bool segment_sequence::intersect_with_circle(float x, float y, float r) const {
	typedef boost::ptr_list<segment>::iterator iter;
	iter end = d->body.end();
	for (iter i = d->body.begin(); i !=  end; ++i) {
		if (i->intersect_with_circle(x, y, r)) return true;
	}
	return false;
}

vec2f segment_sequence::get_head_pos() const {
	return d->head().get_head_pos();
}

vec2f segment_sequence::get_tail_pos() const {
	return d->tail().get_tail_pos();
}

vec2f segment_sequence::get_head_direction() const {
	return d->head().get_head_direction();
}

void segment_sequence::render(skin& s) const {
	render_sequence(d->body, s);
}

void segment_sequence::push_back(std::auto_ptr<segment> s) {
	d->body.push_back(s);
}

bool segment_sequence::is_single_segment() const {
	boost::ptr_list<segment>::const_iterator i = d->body.begin();
	boost::ptr_list<segment>::const_iterator end = d->body.end();

	if (i == end) return false;
	++i;
	return i == end;
}
