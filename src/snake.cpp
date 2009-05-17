#include <cmath>
#include <boost/ptr_container/ptr_list.hpp>
#include <ymse/vec.hpp>
#include "arc.hpp"
#include "line.hpp"
#include "skin.hpp"
#include "snake.hpp"


using ymse::vec2f;


struct snake::impl {
	boost::ptr_list<segment> body;
	int dir;
};


snake::snake() :
	d(new impl)
{
	d->dir = 0;

	vec2f pos(0, -40), dir(0, 1);
	d->body.push_back(new line(pos, dir, 20.f));

	
}

snake::~snake() {
}

vec2f snake::get_head_pos() const {
	return d->body.back().get_head_pos();
}

vec2f snake::get_tail_pos() const {
	return d->body.front().get_tail_pos();
}

vec2f snake::get_head_direction() const {
	return d->body.back().get_head_direction();
}

void snake::set_turn(int dir_) {
	if (d->dir == dir_) return;
	d->dir = dir_;

	segment& head = d->body.back();
	vec2f pos = head.get_head_pos();
	vec2f v_dir = head.get_head_direction();

	if (d->dir == 0) {
		d->body.push_back(new line(pos, v_dir, 0));
		return;
	}

	vec2f normal(-v_dir[1], v_dir[0]);
	float radius = 10.f;

	vec2f center(normal);
	center *= radius * d->dir;
	center += pos;

	vec2f begin_point(pos);
	begin_point -= center;
	float begin_angle = atan2(begin_point[1], begin_point[0]);

	d->body.push_back(new arc(center, radius, begin_angle, begin_angle, d->dir));
}

void snake::head_forward(float length) {
	d->body.back().head_forward(length);
}

float snake::tail_forward(float length) {
	while (!d->body.empty()) {
		length = d->body.front().tail_forward(length);
		if (length < 0.f) break;
		d->body.pop_front();
	}
	return length;
}

void snake::forward(float length) {
	head_forward(length);
	tail_forward(length);
}

void snake::render(skin& s) const {
	vec2f head(get_head_pos());
	vec2f tail(get_tail_pos());

	s.circle(head, 2.5f);
	render_sequence(d->body, s);
	s.circle(tail, 2.5f);
}

bool snake::intersect_with_circle(float x, float y, float r) const {
	typedef boost::ptr_list<segment>::iterator iter;
	iter end = d->body.end();
	for (iter i = d->body.begin(); i !=  end; ++i) {
		if (i->intersect_with_circle(x, y, r)) return true;
	}
	return false;
}

