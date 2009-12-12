#include <algorithm>
#include <cmath>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "skin.hpp"
#include "arc.hpp"

using ymse::vec2f;

arc::arc(
	float x_, float y_,
	float r_,
	float begin_, float end_,
	float direction_
) :
	x(x_), y(y_),
	r(r_),
	begin(begin_), end(end_),
	direction(direction_),
	thickness(2.5)
{
}

arc::arc(
	vec2f pos,
	float r_,
	float begin_, float end_,
	float direction_
) :
	x(pos[0]), y(pos[1]),
	r(r_),
	begin(begin_), end(end_),
	direction(direction_),
	thickness(2.5)
{
}

arc::~arc() {
}

void arc::head_forward(float length) {
	end += direction * length / r;
}

float arc::tail_forward(float length) {
	float current_length = fabs(end - begin) * r;

	if (length < current_length) {
		begin += direction * length / r;
		return -7.f;
	} else {
		return length - current_length;
	}
}

vec2f arc::get_head_pos() const {
	return vec2f(
		x + cos(end) * r,
		y + sin(end) * r
	);
}

vec2f arc::get_tail_pos() const {
	return vec2f(
		x + cos(begin) * r,
		y + sin(begin) * r
	);
}

vec2f arc::get_head_direction() const {
	return vec2f(
		-sin(end) * direction,
		cos(end) * direction
	);
}

static bool fat_arc_intersect_with_circle(
	const vec2f& a_center, float a_r, float thickness, float begin, float end,
	const vec2f& c_center, float c_r
) {
	vec2f AB(c_center); AB -= a_center;

	float dist = AB.length();

	if (dist - c_r > a_r + thickness) return false;
	if (dist + c_r < a_r - thickness) return false;

	float ang = atan2(AB[1], AB[0]);

	vec2f a(begin, end), pi2(2.0*M_PI, 2.0*M_PI);
	if (a[0] > a[1]) std::swap(a[0], a[1]);

	while (a[0] < -M_PI) a += pi2;
	while (a[0] > M_PI) a -= pi2;

	if (a[0] <= ang && ang < a[1]) return true;

	if (a[1] > M_PI) {
		a -= pi2;
		if (a[0] <= ang && ang < a[1]) return true;
	}

	return false;
}

bool arc::intersect_with_circle(const ymse::vec2f& c_center, float c_r) const {
	return fat_arc_intersect_with_circle(vec2f(x, y), r, thickness, begin, end, c_center, c_r);
}

bool arc::intersect_with_circle(const ymse::vec2f& c_center, float c_r, float& skiplength) const {
	if (skiplength <= 0.f) return intersect_with_circle(c_center, c_r);

	float length = fabs(end - begin) * r;
	if (length <= skiplength) {
		skiplength -= length;
		return false;
	}

	float new_end = end - direction * skiplength;
	skiplength = 0;

	return fat_arc_intersect_with_circle(vec2f(x, y), r, thickness, begin, new_end, c_center, c_r);
}

float arc::length() const {
	return fabs(end - begin) * r;
}

void arc::render(skin& s, float head_b) const {
	s.fat_arc(ymse::vec2f(x, y), r, thickness, begin, end, head_b + length(), head_b);
}

ymse::rectf arc::bounding_box() const {

	// This is the bounding box of the entire circle with thickness, not just the arc

	// TODO: Make the box smaller when needed

	ymse::rectf bb = {
		x1: x - r - thickness, y1: y - r - thickness,
		x2: x + r + thickness, y2: y + r + thickness
	};

	return bb;
}
