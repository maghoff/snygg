#include <cmath>
#include <list>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/shared_ptr.hpp>
#include <ymse/vec.hpp>
#include "arc.hpp"
#include "line.hpp"
#include "open_segment.hpp"
#include "segment_sequence.hpp"
#include "segment_heap.hpp"
#include "direction_decider.hpp"

using ymse::vec2f;


static bool are_close(vec2f a, vec2f b) {
	const float EPSILON = 0.01;
	const float EPSILON_SQ = EPSILON * EPSILON;
	return (a-b).square_length() < EPSILON_SQ;
}


namespace heap {

struct line;
struct arc;

struct seg {
	virtual ~seg() {}

	virtual vec2f pos(int) = 0;
	virtual std::auto_ptr<segment> to_segment(int) = 0;

	virtual bool consolidate_dispatcher(int j, seg*, int i) = 0;
	virtual bool consolidate(int i, line*, int j) = 0;
	virtual bool consolidate(int i, arc*, int j) = 0;
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

	bool consolidate_dispatcher(int j, seg* first, int i) {
		return first->consolidate(i, this, j);
	}

	bool consolidate(int i, line* next, int j) {
		if (!are_close(pos(1-i), next->pos(j))) return false;
		//assert(direction about the same);
		p[1-i] = next->p[1-j];
		return true;
	}

	bool consolidate(int, arc*, int) {
		return false;
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

	bool consolidate_dispatcher(int j, seg* first, int i) {
		return first->consolidate(i, this, j);
	}

	bool consolidate(int i, line* next, int j) {
		return false;
	}

	float ang_size() const {
		return fabs(ang[1] - ang[0]);
	}

	float ang_dir(int i) const {
		return ang[1-i] >= ang[i] ? 1 : -1;
	}

	bool consolidate(int i, arc* next, int j) {
		if (!are_close(p, next->p)) return false;
		if (!are_close(pos(1-i), next->pos(j))) return false;
		ang[1-i] += ang_dir(i) * next->ang_size();
		return true;
	}
};

bool consolidate(seg* lhs, int i, seg* rhs, int j) {
	return rhs->consolidate_dispatcher(j, lhs, i);
}

}

typedef boost::shared_ptr<heap::seg> seg_ptr;

struct segment_heap::impl {
	std::list<seg_ptr> segs;
};


segment_heap::segment_heap() : d(new impl) { }
segment_heap::~segment_heap() { }

void segment_heap::line(vec2f a, vec2f b) {
	d->segs.push_back(seg_ptr(new heap::line(a, b)));
}

void segment_heap::arc(vec2f p, float r, float begin, float end) {
	d->segs.push_back(seg_ptr(new heap::arc(p, r, begin, end)));
}

void segment_heap::line(float x1, float y1, float x2, float y2) {
	line(ymse::vec2f(x1, y1), ymse::vec2f(x2, y2));
}

void segment_heap::arc(float x, float y, float r, float begin, float end) {
	arc(ymse::vec2f(x, y), r, begin, end);
}

std::auto_ptr<segment> segment_heap::get_connected_sequence() {
	std::list< std::pair<seg_ptr, int> > s;

	s.push_back(std::make_pair(d->segs.front(), 0));
	d->segs.pop_front();

	bool still_building;
	do {
		still_building = false;
		for (std::list<seg_ptr>::iterator i = d->segs.begin(); i != d->segs.end(); ++i) {
			std::pair<seg_ptr, int> front = s.front();
			std::pair<seg_ptr, int> back = s.back();
			vec2f front_pos = front.first->pos(front.second);
			vec2f back_pos = back.first->pos(1 - back.second);
			bool added = false;
			for (int dir = 0; dir <= 1; ++dir) {
				if (are_close(front_pos, (*i)->pos(dir))) {
					added = true;
					s.push_front(std::make_pair(*i, 1-dir));
					break;
				} else if (are_close(back_pos, (*i)->pos(dir))) {
					added = true;
					s.push_back(std::make_pair(*i, dir));
					break;
				}
			}
			if (added) {
				still_building = true;
				d->segs.erase(i);
				break;
			}
		}
	} while (still_building);

	for (std::list< std::pair<seg_ptr, int> >::iterator i = s.begin(); i != s.end();) {
		std::list< std::pair<seg_ptr, int> >::iterator j = i;
		++j;
		if (j == s.end()) j = s.begin();

		if (i == j) break;

		if (consolidate(i->first.get(), i->second, j->first.get(), j->second)) {
			s.erase(j);
		} else {
			++i;
		}
	}

	std::auto_ptr<segment_sequence> ss(new segment_sequence);
	for (std::list< std::pair<seg_ptr, int> >::iterator i = s.begin(); i != s.end(); ++i) {
		ss->push_back(i->first->to_segment(i->second));
	}

	return std::auto_ptr<segment>(ss.release());
}

std::auto_ptr<segment> segment_heap::to_segment() {
	boost::ptr_list<segment> undecided_segments;
	std::auto_ptr<segment_sequence> decided_segments(new segment_sequence);

	while (!d->segs.empty()) {
		std::auto_ptr<segment> connected_sequence = get_connected_sequence();

		bool is_closed = are_close(connected_sequence->get_head_pos(), connected_sequence->get_tail_pos());

		if (is_closed) {
			undecided_segments.push_back(connected_sequence);
		} else {
			decided_segments->push_back(std::auto_ptr<segment>(new open_segment(connected_sequence.release())));
		}
	}

	direction_decider d(undecided_segments, *decided_segments);
	d.decide_all();

	return std::auto_ptr<segment>(decided_segments.release());
}
