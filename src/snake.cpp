#include <cmath>
#include <ymse/vec.hpp>
#include "arc.hpp"
#include "line.hpp"
#include "skin.hpp"
#include "snake.hpp"

using ymse::vec2f;

snake::snake() :
	dir(0)
{
	vec2f pos(0, -40), dir(0, 1);

	body.push_back(new line(pos, dir, 20.f));

	
}

snake::~snake() {
}

vec2f snake::get_head_pos() const {
	return body.back().get_head_pos();
}

vec2f snake::get_tail_pos() const {
	return body.front().get_tail_pos();
}

void snake::set_turn(int dir_) {
	if (dir == dir_) return;
	dir = dir_;

	segment& head = body.back();
	vec2f pos = head.get_head_pos();
	vec2f v_dir = head.get_head_direction();

	if (dir == 0) {
		body.push_back(new line(pos, v_dir, 0));
		return;
	}

	vec2f normal(-v_dir[1], v_dir[0]);
	float radius = 10.f;

	vec2f center(normal);
	center *= radius * dir;
	center += pos;

	vec2f begin_point(pos);
	begin_point -= center;
	float begin_angle = atan2(begin_point[1], begin_point[0]);

	body.push_back(new arc(center, radius, begin_angle, begin_angle, dir));
}

void snake::forward(float length) {
	body.back().head_forward(length);

	while (true) {
		length = body.front().tail_forward(length);
		if (length < 0.f) break;
		body.pop_front();
	}
}

void snake::render(skin& s) const {
	vec2f head(get_head_pos());
	vec2f tail(get_tail_pos());

	s.circle(head, 2.5f);
	render_sequence(body, s);
	s.circle(tail, 2.5f);
}
