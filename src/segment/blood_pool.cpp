#include <cassert>
#include <cmath>
#include <stdexcept>

#include <rect.hpp>
#include <vec.hpp>
#include "skin.hpp"
#include "blood_pool.hpp"

using la::vec2f;

blood_pool::blood_pool(vec2f pos, float r_) : x(pos[0]), y(pos[1]), r(r_) {
}

blood_pool::~blood_pool() {
}

void blood_pool::head_forward(float length) {
	r = sqrt(r*r + length);
}

float blood_pool::tail_forward(float length) {
	r = sqrt(r*r - length);
	return -1;
}

bool blood_pool::intersect_with_circle(const la::vec2f&, float) const {
	// This is just debris
	return false;
}

vec2f blood_pool::get_head_pos() const {
	return vec2f(x, y);
}

vec2f blood_pool::get_head_direction() const {
	assert(false);
	throw std::logic_error("blood_pool::get_head_direction(...) called");
}

vec2f blood_pool::get_tail_pos() const {
	return vec2f(x, y);
}

vec2f blood_pool::get_tail_direction() const {
	assert(false);
	throw std::logic_error("blood_pool::get_tail_direction(...) called");
}

float blood_pool::length() const {
	return 0;
}

void blood_pool::render(skin& s, float) const {
	render(s);
}

void blood_pool::render(skin& s) const {
	s.blood(vec2f(x, y), r);
}

rectf blood_pool::bounding_box() const {
	rectf bb = {
		x, y,
		x, y
	};

	return bb;
}

int blood_pool::left_hline_intersections(la::vec2f) const {
	assert(!"Not implemented");
	return 0;
}
