#include <cmath>
#include <boost/ptr_container/ptr_list.hpp>
#include <ymse/vec.hpp>
#include "arc.hpp"
#include "line.hpp"
#include "open_segment.hpp"
#include "segment_sequence.hpp"
#include "segment_heap.hpp"

using ymse::vec2f;


namespace heap {

struct seg {
	virtual ~seg() {}

	virtual vec2f pos(int) = 0;
	virtual std::auto_ptr<segment> to_segment(int) = 0;
};

struct line : seg {
	vec2f p[2];

	line(vec2f a, vec2f b) {
		p[0] = a;
		p[1] = b;
	}

	vec2f pos(int i) { return p[i]; }

	std::auto_ptr<segment> to_segment(int i) {
		vec2f d = p[1-i] - p[i];
		float len = d.length();
		return std::auto_ptr<segment>(new ::line(p[i], d*(1.f/len), len));
	}
};

struct arc : seg {
	vec2f p;
	float rad;
	float ang[2];

	arc(vec2f p_, float rad_, float begin, float end) :
		p(p_), rad(rad_)
	{
		ang[0] = begin;
		ang[1] = end;
	}

	vec2f pos(int i) {
		return p + rad * vec2f(cos(ang[i]), sin(ang[i]));
	}

	std::auto_ptr<segment> to_segment(int i) {
		float dir = ang[1-i] >= ang[i] ? 1 : -1;
		return std::auto_ptr<segment>(new ::arc(p, rad, ang[i], ang[1-i], dir));
	}
};

}

struct segment_heap::impl {
	boost::ptr_list<heap::seg> segs;
};


segment_heap::segment_heap() : d(new impl) { }
segment_heap::~segment_heap() { }

void segment_heap::line(vec2f a, vec2f b) {
	d->segs.push_back(new heap::line(a, b));
}

void segment_heap::arc(vec2f p, float r, float begin, float end) {
	d->segs.push_back(new heap::arc(p, r, begin, end));
}

void segment_heap::line(float x1, float y1, float x2, float y2) {
	line(ymse::vec2f(x1, y1), ymse::vec2f(x2, y2));
}

void segment_heap::arc(float x, float y, float r, float begin, float end) {
	arc(ymse::vec2f(x, y), r, begin, end);
}

std::auto_ptr<segment> segment_heap::to_segment() {
	std::auto_ptr<segment_sequence> s(new segment_sequence);

	while (!d->segs.empty()) {
		s->push_back(d->segs.begin()->to_segment(0));
		d->segs.pop_front();
	}

	return std::auto_ptr<segment>(new open_segment(s.release()));
}
