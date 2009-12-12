#include <SDL.h>
#include <cmath>
#include <boost/ptr_container/ptr_vector.hpp>
#include <ymse/rect.hpp>
#include "arc.hpp"
#include "line.hpp"
#include "board.hpp"


struct board::impl {
	boost::ptr_vector<segment> b;
};


namespace {

void box(boost::ptr_vector<segment>& b, const ymse::rectf& rc, float r) {
	const float &x1 = rc.x1, &y1 = rc.y1, &x2 = rc.x2, &y2 = rc.y2;
	float w = rc.width(), h = rc.height();

	b.push_back(new line(x1 + r, y1    ,  1,  0, w - 2*r));
	b.push_back(new arc (x2 - r, y1 + r,  r,  M_PI * -0.5, M_PI *  0.0, 1));
	b.push_back(new line(x2    , y1 + r,  0,  1, h - 2*r));
	b.push_back(new arc (x2 - r, y2 - r,  r,  M_PI *  0.0, M_PI *  0.5, 1));
	b.push_back(new line(x2 - r, y2    , -1,  0, w - 2*r));
	b.push_back(new arc (x1 + r, y2 - r,  r,  M_PI *  0.5, M_PI *  1.0, 1));
	b.push_back(new line(x1    , y2 - r,  0, -1, h - 2*r));
	b.push_back(new arc (x1 + r, y1 + r,  r,  M_PI * -1.0, M_PI * -0.5, 1));
}

}


board::board() :
	d(new impl)
{
	d->b.reserve(16);

	// Classic board:
	/*ymse::rectf bb = {
		x1: -200, y1: -50,
		x2: 200, y2: 50
	};*/

	// 16:9 YouTube-friendly board:
	ymse::rectf bb = {
		x1: -120, y1: -65,
		x2: 120, y2: 65
	};

	// Extremists board
	/*ymse::rectf bb = {
		x1: -24, y1: -48,
		x2: 24, y2: 0
	};*/

	ymse::rectf inner_box = {
		x1: -100, y1: 0,
		x2: 100, y2: 45
	};

	box(d->b, bb, 10);
	box(d->b, inner_box, 10);
}

board::~board() {
}

void board::render(skin& s) const {
	render_sequence(d->b, s, SDL_GetTicks() * 0.0002);
}

bool board::intersect_with_circle(const ymse::vec2f& p, float r) const {
	typedef boost::ptr_vector<segment>::const_iterator iter;
	iter end = d->b.end();
	for (iter i = d->b.begin(); i != end; ++i) {
		if (i->intersect_with_circle(p, r)) return true;
	}
	return false;
}

ymse::rectf board::bounding_box() const {
	ymse::rectf bb = d->b[0].bounding_box();

	typedef boost::ptr_vector<segment>::const_iterator iter;
	iter end = d->b.end();
	for (iter i = d->b.begin(); i != end; ++i) {
		bb = ymse::bounding_box(bb, i->bounding_box());
	}

	return bb;
}
