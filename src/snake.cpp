#include <cmath>
#include <ymse/vec.hpp>
#include "arc.hpp"
#include "blood_pool.hpp"
#include "line.hpp"
#include "scored_point.hpp"
#include "segment_sequence.hpp"
#include "skin.hpp"
#include "snake.hpp"


using ymse::vec2f;


struct snake::impl {
	segment_sequence body;
	int dir;
};


snake::snake() :
	d(new impl)
{
	d->dir = 0;

	vec2f pos(0, -40), dir(0, 1);
	d->body.push_back(segment_ptr(new line(pos, dir, 20.f)));
}

snake::~snake() {
}

void snake::score(float amount) {
	vec2f pos = d->body.get_head_pos();
	vec2f v_dir = d->body.get_head_direction();

	const float min_r = 2.5;
	float r = sqrt(min_r * min_r + amount);

	d->body.push_back(segment_ptr(new scored_point(pos, r, min_r, v_dir)));
	int old_dir = d->dir;
	d->dir = -100; //< invalid value
	set_turn(old_dir);
}

void snake::set_turn(int dir_) {
	if (d->dir == dir_) return;
	d->dir = dir_;

	vec2f pos = d->body.get_head_pos();
	vec2f v_dir = d->body.get_head_direction();

	if (d->dir == 0) {
		d->body.push_back(segment_ptr(new line(pos, v_dir, 0)));
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

	d->body.push_back(segment_ptr(new arc(center, radius, begin_angle, begin_angle, d->dir)));
}

void snake::forward(float length) {
	d->body.head_forward(length);
	d->body.tail_forward(length);
}

void snake::render(skin& s) const {
	vec2f head(d->body.get_head_pos());
	vec2f tail(d->body.get_tail_pos());

	s.circle(head, 2.5f);
	d->body.render(s);
	s.circle(tail, 2.5f);
}

bool snake::crashes_with(intersectable_with_circle& object) const {
	vec2f head(d->body.get_head_pos());
	return object.intersect_with_circle(head[0], head[1], 2.5f);
}

void snake::crack_head() {
	d->body.push_back(segment_ptr(new blood_pool(d->body.get_head_pos(), 2.5f)));
}
