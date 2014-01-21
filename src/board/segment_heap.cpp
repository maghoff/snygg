#include <cmath>
#include <list>
#include <memory>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "arc.hpp"
#include "contour_segment.hpp"
#include "line.hpp"
#include "open_segment.hpp"
#include "segment_sequence.hpp"
#include "segment_heap.hpp"

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
	virtual std::unique_ptr<segment> to_segment(int) = 0;

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

	std::unique_ptr<segment> to_segment(int i) {
		vec2f d = p[1-i] - p[i];
		float len = d.length();
		return std::unique_ptr<segment>(new ::line(p[i], d*(1.f/len), len));
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

	std::unique_ptr<segment> to_segment(int i) {
		float dir = ang[1-i] >= ang[i] ? 1 : -1;
		return std::unique_ptr<segment>(new ::arc(p, rad, ang[i], ang[1-i], dir));
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

typedef std::shared_ptr<heap::seg> seg_ptr;

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

std::auto_ptr<segment> segment_heap::get_a_connected_sequence() {
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

	std::unique_ptr<segment_sequence> ss(new segment_sequence);
	for (auto& x : s) {
		ss->push_back(x.first->to_segment(x.second));
	}

	return std::auto_ptr<segment>(ss.release());
}

std::unique_ptr<segment> segment_heap::to_segment() {
	bool has_closed_segments = false;

	std::unique_ptr<segment_sequence> s(new segment_sequence);

	while (!d->segs.empty()) {
		std::unique_ptr<segment> connected_sequence = get_a_connected_sequence();

		bool is_closed = are_close(connected_sequence->get_head_pos(), connected_sequence->get_tail_pos());

		if (is_closed) {
			s->push_back(std::unique_ptr<segment>(new contour_segment(connected_sequence.release())));
			has_closed_segments = true;
		} else {
			s->push_back(std::unique_ptr<segment>(new open_segment(connected_sequence.release())));
		}
	}

	// The game will hang if there are no closed segments
	if (!has_closed_segments) {
		using ymse::vec2f;
		const double tau = 2. * M_PI;
		const double margin = 20.;
		const double radius = 10.;

		ymse::rectf b = s->bounding_box();
		b.x1 -= margin;
		b.y1 -= margin;
		b.x2 += margin;
		b.y2 += margin;

		std::unique_ptr<segment_sequence> box(new segment_sequence);
		box->push_back(new ::line(vec2f(b.x2, b.y1 + radius), vec2f(0.f, 1.f), b.y2 - b.y1 - 2.*radius));
		box->push_back(new ::arc(vec2f(b.x2 - radius, b.y2 - radius), radius, tau * 0./4., tau * 1./4., 1.));
		box->push_back(new ::line(vec2f(b.x2 - radius, b.y2), vec2f(-1.f, 0.f), b.x2 - b.x1 - 2.*radius));
		box->push_back(new ::arc(vec2f(b.x1 + radius, b.y2 - radius), radius, tau * 1./4., tau * 2./4., 1.));
		box->push_back(new ::line(vec2f(b.x1, b.y2 - radius), vec2f(0.f, -1.f), b.y2 - b.y1 - 2.*radius));
		box->push_back(new ::arc(vec2f(b.x1 + radius, b.y1 + radius), radius, tau * 2./4., tau * 3./4., 1.));
		box->push_back(new ::line(vec2f(b.x1 + radius, b.y1), vec2f(1.f, 0.f), b.x2 - b.x1 - 2.*radius));
		box->push_back(new ::arc(vec2f(b.x2 - radius, b.y1 + radius), radius, tau * 3./4., tau * 4./4., 1.));

		s->push_back(std::unique_ptr<segment>(new contour_segment(box.release())));
	}

	return std::move(s);
}
