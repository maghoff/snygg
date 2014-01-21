#include <algorithm>
#include <cassert>
#include "gl.h"
#include "pixels_per_unit_listener.hpp"
#include "gl_box_reshaper.hpp"

namespace ymse {

gl_box_reshaper::gl_box_reshaper() :
	ppu_listener(0)
{
}

gl_box_reshaper::~gl_box_reshaper() {
}

void gl_box_reshaper::update_listener() {
	if (ppu_listener) {
		double w_ppu = width / (x2 - x1);
		double h_ppu = height / (y2 - y1);
		double ppu = std::min(w_ppu, h_ppu);

		ppu_listener->set_pixels_per_unit(ppu);
	}
}

void gl_box_reshaper::set_projection_matrix() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glScaled(height/(double)width, 1.0, 1.0);

	double w = 2.0*width/(double)height;
	double h = 2.0;

	double w_scale = w / (x2 - x1);
	double h_scale = h / (y2 - y1);

	if (w_scale == 0.0) w_scale = 1.0;
	if (h_scale == 0.0) h_scale = 1.0;
	if (w_scale < 0.0) w_scale = -w_scale;
	if (h_scale < 0.0) h_scale = -h_scale;
	assert(w_scale > 0.0);
	assert(h_scale > 0.0);

	double scale = std::min(w_scale, h_scale);

	glScaled(scale, scale, 1.0);

	glTranslated(-(x1 + x2) / 2.0, -(y1 + y2) / 2.0, 0.0);

	update_listener();
}

void gl_box_reshaper::reshape(int width_, int height_) {
	width = width_;
	height = height_;

	assert(width > 0);
	assert(height > 0);

	glViewport(0, 0, width, height);
	set_projection_matrix();
}

void gl_box_reshaper::set_box(
	double x1_, double y1_,
	double x2_, double y2_
) {
	x1 = x1_;
	y1 = y1_;
	x2 = x2_;
	y2 = y2_;

	set_projection_matrix();
}

void gl_box_reshaper::set_pixels_per_unit_listener(pixels_per_unit_listener* ppu_listener_) {
	ppu_listener = ppu_listener_;
	update_listener();
}

}
