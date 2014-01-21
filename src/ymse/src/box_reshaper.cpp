#include <cassert>
#include <algorithm>
#include "box_reshaper.hpp"
#include "matrix2d_homogenous.hpp"

namespace ymse {

box_reshaper::box_reshaper() {
	set_box(-1, -1, 1, 1);
}

box_reshaper::~box_reshaper() { }

void box_reshaper::calculate_transformation() {
	using namespace matrix2d::homogenous;

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

	const double s = std::min(w_scale, h_scale);
	const double ratio = height/(double)width;
	const double tx = -(x1 + x2) / 2.0, ty = -(y1 + y2) / 2.0;

	t =
		scale(ratio, 1.0) *
		scale(s, s) *
		translate(tx, ty);

	t_inv =
		translate(-tx, -ty) *
		scale(1./s, 1./s) *
		scale(1./ratio, 1.0);


	double w_ppu = width / (x2 - x1);
	double h_ppu = height / (y2 - y1);
	ppu = std::min(w_ppu, h_ppu);
}

void box_reshaper::reshape(int width_, int height_) {
	width = width_;
	height = height_;

	assert(width > 0);
	assert(height > 0);

	calculate_transformation();
}

void box_reshaper::set_box(
	double x1_, double y1_,
	double x2_, double y2_
) {
	x1 = x1_;
	y1 = y1_;
	x2 = x2_;
	y2 = y2_;

	calculate_transformation();
}

double box_reshaper::get_pixels_per_unit() const {
	return ppu;
}

const matrix33f& box_reshaper::get_transformation() const {
	return t;
}

const matrix33f& box_reshaper::get_inverse_transformation() const {
	return t_inv;
}

} // namespace ymse
