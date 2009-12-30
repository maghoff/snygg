#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "extender.hpp"

struct extender::impl {
	ymse::vec2f pos, dir;
	float length;
};

extender::extender(
	ymse::vec2f pos,
	ymse::vec2f dir,
	float length
) :
	d(new impl)
{
	d->pos = pos;
	d->dir = dir;
	d->length = length;
}

extender::~extender() {
}

void extender::head_forward(float l) {
	assert(!"extender::head_forward(...) called, but it does not make sense");
	d->length += l;
}

float extender::tail_forward(float l) {
	if (l >= d->length) {
		float rest = l - d->length;
		d->length = 0;
		return rest;
	} else {
		d->length -= l;
		return -1.;
	}
}

ymse::vec2f extender::get_head_pos() const {
	return d->pos;
}

ymse::vec2f extender::get_tail_pos() const {
	return d->pos;
}

ymse::vec2f extender::get_head_direction() const {
	return d->dir;
}

float extender::length() const {
	// This length is used to calculate the b-coordinate
	return 0;
}

void extender::render(skin&, float head_b) const {
}

ymse::rectf extender::bounding_box() const {
	ymse::rectf rc = {
		x1: d->pos[0], y1: d->pos[1],
		x2: d->pos[0], y2: d->pos[1]
	};
	return rc;
}

int extender::left_hline_intersections(ymse::vec2f p) const {
	return 0;
}

bool extender::intersect_with_circle(const ymse::vec2f&, float r) const {
	return false;
}

bool extender::intersect_with_circle(const ymse::vec2f&, float r, float& skiplength) const {
	return false;
}