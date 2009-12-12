#include <cassert>
#include <cmath>
#include <stdexcept>
#include <GL/gl.h>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "blood_pool.hpp"
#include "skin.hpp"

using ymse::vec2f;

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

bool blood_pool::intersect_with_circle(const ymse::vec2f&, float) const {
	// This is just debris
	return false;
}

vec2f blood_pool::get_head_pos() const {
	return vec2f(x, y);
}

vec2f blood_pool::get_tail_pos() const {
	return vec2f(x, y);
}

vec2f blood_pool::get_head_direction() const {
	assert(false);
	throw std::logic_error("blood_pool::get_head_direction(...) called");
}

float blood_pool::length() const {
	return 0;
}

void blood_pool::render(skin& s, float) const {
	render(s);
}

void blood_pool::render(skin& s) const {
	// I am not sure that it is proper of me to set the colour here.

	glColor4f(1.f, 0.f, 0.f, 1.f);
	s.circle(vec2f(x, y), r);
	glColor4f(1.f, 1.f, 1.f, 1.f);
}

ymse::rectf blood_pool::bounding_box() const {
	ymse::rectf bb = {
		x1: x, y1: y,
		x2: x, y2: y
	};

	return bb;
}

int blood_pool::left_hline_intersections(ymse::vec2f) const {
	assert(!"Not implemented");
	return 0;
}
