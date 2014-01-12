#include <cassert>
#include <cmath>
#include <stdexcept>
#include <ymse/geometry_intersection.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "skin.hpp"
#include "scored_point.hpp"

using ymse::vec2f;

scored_point::scored_point(
	vec2f pos, float r_,
	float min_r_,
	vec2f dir
) :
	x(pos[0]), y(pos[1]), r(r_),
	min_r(min_r_),
	dx(dir[0]), dy(dir[1])
{
}

scored_point::~scored_point() {
}

void scored_point::head_forward(float) {
	assert(false);
	throw std::logic_error("scored_point::head_forward(...) called");
}

float scored_point::tail_forward(float length) {
	float l = r*r - min_r*min_r;

	if (length < l) {
		r = sqrt(r*r - length);
		return -7.f;
	} else {
		return length - l;
	}
}

bool scored_point::intersect_with_circle(const ymse::vec2f& p2, float r2) const {
	return ymse::intersect::circle_with_circle(ymse::vec2f(x, y), r, p2, r2);
}

bool scored_point::intersect_with_circle(const ymse::vec2f& p2, float r2, float& skiplength) const {
	return
		skiplength <= 0.f &&
		intersect_with_circle(p2, r2)
	;
}


vec2f scored_point::get_head_pos() const {
	return vec2f(x, y);
}

vec2f scored_point::get_head_direction() const {
	return vec2f(dx, dy);
}

vec2f scored_point::get_tail_pos() const {
	return get_head_pos();
}

vec2f scored_point::get_tail_direction() const {
	return get_head_direction();
}

float scored_point::length() const {
	return 0;
}

void scored_point::render(skin& s, float head_b) const {
	s.circle(vec2f(x, y), r);
}

ymse::rectf scored_point::bounding_box() const {
	ymse::rectf bb = {
		x - r, y - r,
		x + r, y + r
	};

	return bb;
}

int scored_point::left_hline_intersections(ymse::vec2f) const {
	assert(!"Not implemented");
	return 0;
}
