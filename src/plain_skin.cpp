#include <algorithm>
#include <cmath>
#include <GL/gl.h>
#include <ymse/vec.hpp>
#include "plain_skin.hpp"

plain_skin::plain_skin() :
	detail_modifier(1.f)
{
}

void plain_skin::circle(ymse::vec2f p, float r) {
	float step_size = detail_modifier / r;

	step_size = std::min<float>(step_size, M_PI * 2. / 8.); // At least 8 vertices

	glBegin(GL_TRIANGLE_FAN);
	for (float d = 0.f; d < M_PI * 2.f; d += step_size) {
		glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));
	}
	glEnd();
}

void plain_skin::fat_arc(float x, float y, float r, float t, float begin, float end) {
	float r1 = r-t, r2 = r+t;
	if (begin > end) std::swap(begin, end);

	float step_size = detail_modifier / r2;
	step_size = std::min<float>(step_size, M_PI * 2. / 8.); // At least as detailed as 8 vertices to a circle

	glBegin(GL_TRIANGLE_STRIP);

	for (float d = begin; d < end; d += step_size) {
		glVertex2f(x + r1 * cos(d), y + r1 * sin(d));
		glVertex2f(x + r2 * cos(d), y + r2 * sin(d));
	}

	glVertex2f(x + r1 * cos(end), y + r1 * sin(end));
	glVertex2f(x + r2 * cos(end), y + r2 * sin(end));

	glEnd();
}

void plain_skin::fat_line(float x1, float y1, float dx, float dy, float len, float t) {
	// Calculate normal * thickness:
	float nx = dy * t, ny = -dx * t;
	float x2 = x1 + dx*len, y2 = y1 + dy * len;

	glBegin(GL_QUADS);
	glVertex2f(x1 + nx, y1 + ny);
	glVertex2f(x2 + nx, y2 + ny);
	glVertex2f(x2 - nx, y2 - ny);
	glVertex2f(x1 - nx, y1 - ny);
	glEnd();
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

void plain_skin::set_pixels_per_unit(float pixels_per_unit) {
	detail_modifier = 1. / (pixels_per_unit * level_of_detail);
}
