#include <cassert>
#include <algorithm>
#include <cmath>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "../skin/skin.hpp"
#include "../complex_polygon_triangulator.hpp"
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
	assert(r >= 0);
//	assert((end - begin) * (direction) >= 0);
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

vec2f arc::get_head_direction() const {
	return vec2f(
		-sin(end) * direction,
		cos(end) * direction
	);
}

vec2f arc::get_tail_pos() const {
	return vec2f(
		x + cos(begin) * r,
		y + sin(begin) * r
	);
}

vec2f arc::get_tail_direction() const {
	return vec2f(
		-sin(begin) * direction,
		cos(begin) * direction
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

	float new_end = end - direction * (skiplength / r);
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
	using namespace ymse;

	const float r1 = r - thickness, r2 = r + thickness;
	vec2f b1(x + cos(begin) * r1, y + sin(begin) * r1);
	vec2f b2(x + cos(begin) * r2, y + sin(begin) * r2);
	vec2f e1(x + cos(end) * r1, y + sin(end) * r1);
	vec2f e2(x + cos(end) * r2, y + sin(end) * r2);

	rectf bb = { b1[0], b1[1], b1[0], b1[1] };
	bb = ymse::bounding_box(bb, b2);
	bb = ymse::bounding_box(bb, e1);
	bb = ymse::bounding_box(bb, e2);

	const float a1 = 0, a2 = M_PI * 1./2., a3 = M_PI * 2./2., a4 = M_PI * 3./2.;

	float bg = begin, en = end;
	if (bg > en) std::swap(bg, en);
	while (en > 2. * M_PI) { bg -= 2. * M_PI; en -= 2. * M_PI; }
	while (en < 0) { bg += 2. * M_PI; en += 2. * M_PI; }
	if (bg <= a1 && en >= a1) bb = ymse::bounding_box(bb, vec2f(x + r2, y));
	while (en < a2) { bg += 2. * M_PI; en += 2. * M_PI; }
	if (bg <= a2 && en >= a2) bb = ymse::bounding_box(bb, vec2f(x, y + r2));
	while (en < a3) { bg += 2. * M_PI; en += 2. * M_PI; }
	if (bg <= a3 && en >= a3) bb = ymse::bounding_box(bb, vec2f(x - r2, y));
	while (en < a4) { bg += 2. * M_PI; en += 2. * M_PI; }
	if (bg <= a4 && en >= a4) bb = ymse::bounding_box(bb, vec2f(x, y - r2));

	return bb;
}

int arc::left_hline_intersections(ymse::vec2f p) const {
	if (p[1] < y - r) return 0;
	if (p[1] > y + r) return 0;

	float dy = (p[1] - y) / r;
	float ang1 = asin(dy), ang2 = M_PI - ang1;
	if (ang1 < 0) ang1 += 2.0 * M_PI;

	float x1 = x + cos(ang1) * r, x2 = x + cos(ang2) * r;

	// (x1, p[1]) and (x2, p[1]) are the two possible intersection points.
	// Each point counts iff x < p[0] and the angle is included in this arc:

	float b1 = begin, e1 = end;
	if (b1 > e1) std::swap(b1, e1);

	while (e1 < 0) {
		b1 += 2. * M_PI;
		e1 += 2. * M_PI;
	}

	while (b1 >= 0) {
		b1 -= 2. * M_PI;
		e1 -= 2. * M_PI;
	}

	float b2 = b1 + 2. * M_PI, e2 = e1 + 2. * M_PI;

	int n = 0;

	assert(b1 < e1);
	assert(-2.01*M_PI <= b1 && b1 < 0);
	assert(e1 < 2. * M_PI);
	assert(0 <= ang1 && ang1 < 2. * M_PI);
	assert(0 <= ang2 && ang2 < 2. * M_PI);

	if (x1 < p[0]) {
		if (b1 <= ang1 && ang1 < e1) n++;
		else if (b2 <= ang1 && ang1 < e2) n++;
	}

	if (x2 < p[0]) {
		if (b1 <= ang2 && ang2 < e1) n++;
		else if (b2 <= ang2 && ang2 < e2) n++;
	}

	return n;
}

void arc::add_to_triangulator(complex_polygon_triangulator* triangulator) const {
	double inner = r - thickness, outer = r + thickness;
	double sigma = M_PI - 2. * asin(inner / outer);

	int n = ceil(fabs((end - begin) / sigma));
	double delta = (end - begin) / n;
	for (int i=0; i<=n; ++i) {
		double ang = begin + delta * i;
		triangulator->point(ymse::vec2f(
			x + outer * cos(ang),
			y + outer * sin(ang)
		));
	}
}
