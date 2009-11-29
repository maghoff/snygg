#include <cassert>
#include <cmath>
#include <stdexcept>
#include <ymse/vec.hpp>
#include "scored_point.hpp"
#include "skin.hpp"

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

bool scored_point::intersect_with_circle(const ymse::vec2f&, float) const {

	// Stub this out. It will be a bit difficult to implement it properly
	// without having the player crash with it whenever it scores.

	return false;
}

vec2f scored_point::get_head_pos() const {
	return vec2f(x, y);
}

vec2f scored_point::get_tail_pos() const {
	return vec2f(x, y);
}

vec2f scored_point::get_head_direction() const {
	return vec2f(dx, dy);
}

float scored_point::length() const {
	return 0;
}

void scored_point::render(skin& s, float head_b) const {
	s.circle(vec2f(x, y), r);
}
