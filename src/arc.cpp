#include <algorithm>
#include <cmath>
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

bool arc::intersect_with_circle(const ymse::vec2f& B, float r2) const {
	vec2f A(x, y);
	vec2f AB(B); AB -= A;

	float dist = AB.length();

	if (dist - r2 > r + thickness) return false;
	if (dist + r2 < r - thickness) return false;

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

bool arc::intersect_with_circle(const ymse::vec2f& B, float r2, float& skiplength) const {
	if (skiplength <= 0.f) return intersect_with_circle(B, r2);

	float length = fabs(end - begin) * r;
	if (length <= skiplength) {
		skiplength -= length;
		return false;
	}

	vec2f A(x, y);
	vec2f AB(B); AB -= A;

	float dist = AB.length();

	if (dist - r2 > r + thickness) return false;
	if (dist + r2 < r - thickness) return false;

	float ang = atan2(AB[1], AB[0]);

	vec2f a(begin, end - direction * skiplength), pi2(2.0*M_PI, 2.0*M_PI);
	skiplength = 0.f;
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

void arc::render(skin& s) const {
	s.fat_arc(x, y, r, thickness, begin, end);
}
