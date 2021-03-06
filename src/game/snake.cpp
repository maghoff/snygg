#include <cmath>
#include <random>
#include <geometry_intersection.hpp>
#include <vec.hpp>
#include "arc.hpp"
#include "blood_pool.hpp"
#include "line.hpp"
#include "segment_sequence.hpp"
#include "skin.hpp"
#include "digesting_growth_policy.hpp"
#include "immediate_growth_policy.hpp"
#include "item_container.hpp"
#include "player.hpp"
#include "snake.hpp"


using la::vec2f;


struct snake::impl {
	item_container& ic;

	std::unique_ptr<growth_policy> grow;

	segment_sequence body;

	segment* head;
	segment* tail;

	float speed;
	int dir;
	bool is_dying = false;
	int rigor_mortis;

	float front_b;

	impl(item_container& ic_) : ic(ic_) { }
};


snake::snake(item_container& ic, float speed, vec2f pos) :
	d(new impl(ic))
{
	d->grow.reset(new immediate_growth_policy);

	d->speed = speed;
	d->dir = -100; //< Invalid direction, forces set_turn to add a segment

	vec2f dir(0, 1);
	d->body.push_back(d->grow->growth_segment(pos, dir, 20.f));

	d->tail = d->head = &d->body;

	d->front_b = 0;
}

snake::~snake() {
}

void snake::score(float amount) {
	d->grow->grow(d->body, amount);

	// TODO: Potential cleanup.
	// We don't need to set_turn when using immediate_growth_policy
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
	if (d->head != d->tail) d->front_b += length;

	d->head->head_forward(length);

	float l = d->tail->tail_forward(length);
	if (l >= 0.f) {
		// The tail has moved entirely into the blood pool.
		d->head->tail_forward(l);
		die();
	}
}

void snake::move() {
	forward(d->speed);
	if (d->is_dying) {
		if (d->rigor_mortis) {
			if (!--d->rigor_mortis) d->speed = 0.01;
		} else {
			d->speed *= 1.02;
		}
	}
}

void snake::render(skin& s) const {
	vec2f head(d->body.get_head_pos()), head_dir(d->body.get_head_direction());
	s.cap(head, atan2(head_dir[1], head_dir[0]), M_PI * 0.5, d->front_b);

	d->body.render(s, d->front_b);

	vec2f tail(d->body.get_tail_pos()), tail_dir(d->body.get_tail_direction());
	s.cap(tail, atan2(tail_dir[1], tail_dir[0]), M_PI * 1.5, d->front_b + d->body.length());
}

bool snake::crashes_with(const intersectable_with_circle& object) const {
	vec2f head(d->body.get_head_pos());

	if (&object == this) {
		return d->body.intersect_with_self(head, 2.5f);
	}
	return object.intersect_with_circle(head, 2.5f);
}

bool snake::intersect_with_circle(const la::vec2f& p, float r) const {
	vec2f head(d->body.get_head_pos());
	vec2f tail(d->body.get_tail_pos());

	return
		circle_with_circle(p, r, head, 2.5f) ||
		circle_with_circle(p, r, tail, 2.5f) ||
		d->body.intersect_with_circle(p, r)
	;
}

void snake::crack_head() {
	auto head_pos = d->head->get_head_pos();
	std::unique_ptr<blood_pool> p(new blood_pool(head_pos, 2.5f));
	d->head = p.get();
	d->ic.add_renderable(std::move(p));

	static std::default_random_engine generator;
	std::normal_distribution<float> offsetg(0.0,3.0);
	std::normal_distribution<float> radius(1.5,1.0);

	for (int i=0; i<5; ++i) {
		la::vec2f offset(offsetg(generator), offsetg(generator));
		float r = 3 - 0.02 * offset.square_length();
		if (r <= 0.1) { --i; continue; }
		d->ic.add_renderable(std::unique_ptr<renderable>(new blood_pool(head_pos + offset, r)));
	}

	d->is_dying = true;
	d->rigor_mortis = 80;
	d->speed = 0;
}

void snake::hit_by(player& p) {
	p.die();
}
