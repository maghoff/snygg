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

arc::~arc() {
}

void arc::head_forward(float length) {
	end += direction * length / r;
}

float arc::tail_forward(float length) {
	float current_length = fabs(end - begin) * r;

	if (length < current_length) {
		begin += direction * length / r;
		return 0;
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

bool arc::intersect_with_circle(float x2, float y2, float r2) const {
	vec2f A(x, y), B(x2, y2);
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

void arc::render(skin& s) const {
	s.fat_arc(x, y, r, thickness, begin, end);
}
