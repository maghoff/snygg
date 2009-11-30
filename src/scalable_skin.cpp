#include <algorithm>
#include <cmath>
#include "scalable_skin.hpp"

scalable_skin::scalable_skin() :
	detail_modifier(1.f)
{
}

float scalable_skin::get_step_size(float r) {
	return std::min<float>(detail_modifier / r, M_PI * 2. / 8.);
}

/* For a circle:
  step_size
	= 2pi / desired_number_of_vertices
	= 2pi / (circumference_in_pixels * level_of_detail)
	= 2pi / (circumference_in_screen_units * pixels_per_unit * level_of_detail)
	= 2pi / (2pi * r * pixels_per_unit * level_of_detail)
	= 1 / (r * pixels_per_unit * level_of_detail)
	= (1 / r) * (1 / (pixels_per_unit * level_of_detail))
	= (1 / r) * detail_modifier
	= detail_modifier / r
*/

const float level_of_detail = 0.4f;

void scalable_skin::set_pixels_per_unit(float pixels_per_unit) {
	detail_modifier = 1. / (pixels_per_unit * level_of_detail);
}
