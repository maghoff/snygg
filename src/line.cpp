#include <ymse/vec.hpp>
#include "skin.hpp"
#include "line.hpp"

using ymse::vec2f;

line::line(
	float x_, float y_,
	float dx_, float dy_,
	float length_
) :
	x(x_), y(y_),
	dx(dx_), dy(dy_),
	length(length_),
	thickness(2.5)
{
}

line::line(
	ymse::vec2f pos,
	ymse::vec2f dir,
	float length_
) :
	x(pos[0]), y(pos[1]),
	dx(dir[0]), dy(dir[1]),
	length(length_),
	thickness(2.5)
{
}

line::~line() {
}

void line::head_forward(float l) {
	length += l;
}

float line::tail_forward(float l) {
	if (l < length) {
		length -= l;
		vec2f p(x, y), d(dx, dy);
		d *= l;
		p += d;
		x = p[0];
		y = p[1];
		return -7.f;
	} else {
		return l - length;
	}
}

static bool fat_line_intersect_with_circle(
	const vec2f& A, float dx, float dy, float length, float thickness,
	const vec2f& c_center, float c_r
) {
	vec2f AB(c_center); AB -= A;

	float u = AB[0] * dx + AB[1] * dy;
	if (u < 0 || u >= length) return false;

	vec2f normal(dy, -dx);
	float v = AB[0]*normal[0] + AB[1]*normal[1];

	float r = c_r + thickness;
	if (v < -r || v > r) return false;

	return true;
}

bool line::intersect_with_circle(const ymse::vec2f& B, float r2) const {
	return fat_line_intersect_with_circle(vec2f(x, y), dx, dy, length, thickness, B, r2);
}

bool line::intersect_with_circle(const ymse::vec2f& B, float r2, float& skiplength) const {
	if (skiplength <= 0.f) return intersect_with_circle(B, r2);

	if (length <= skiplength) {
		skiplength -= length;
		return false;
	}

	float l2 = length - skiplength;
	skiplength = 0;

	return fat_line_intersect_with_circle(vec2f(x, y), dx, dy, l2, thickness, B, r2);
}

ymse::vec2f line::get_head_pos() const {
	vec2f v(dx, dy);
	v *= length;
	v += vec2f(x, y);
	return v;
}

ymse::vec2f line::get_tail_pos() const {
	return vec2f(x, y);
}

ymse::vec2f line::get_head_direction() const {
	return vec2f(dx, dy);
}

void line::render(skin& s) const {
	s.fat_line(ymse::vec2f(x, y), ymse::vec2f(dx, dy), length, thickness);
}
