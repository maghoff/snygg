#include "plain_skin.hpp"

#include <algorithm>
#include <cmath>
#include <GL/glew.h>

#include <rect.hpp>
#include <vec.hpp>
#include <matrix2d_homogenous.hpp>
#include "draw_complex_polygon.hpp"

plain_skin::plain_skin() {
}

void plain_skin::set_transformation(const la::matrix33f& transform) {
	glLoadMatrixf(la::matrix2d::homogenous::as_3d_homogenous(transform.transposed()).v);
}

void plain_skin::circle(la::vec2f p, float r) {
	float step_size = get_step_size(r);

	glBegin(GL_TRIANGLE_FAN);
	for (float d = 0.f; d < M_PI * 2.f; d += step_size) {
		glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));
	}
	glEnd();
}

void plain_skin::blood(la::vec2f p, float r) {
	glColor4f(1, 0, 0, 1);
	circle(p, r);
	glColor4f(1, 1, 1, 1);
}

void plain_skin::fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
	float r1 = r-t, r2 = r+t;
	float step_size = get_step_size(r2);
	if (begin > end) std::swap(begin, end);

	float &x(p[0]), &y(p[1]);

	glBegin(GL_TRIANGLE_STRIP);

	for (float d = begin; d < end; d += step_size) {
		glVertex2f(x + r1 * cos(d), y + r1 * sin(d));
		glVertex2f(x + r2 * cos(d), y + r2 * sin(d));
	}

	glVertex2f(x + r1 * cos(end), y + r1 * sin(end));
	glVertex2f(x + r2 * cos(end), y + r2 * sin(end));

	glEnd();
}

void plain_skin::fat_line(la::vec2f p, la::vec2f d, float len, float t, float b_begin, float b_end) {
	float &x1(p[0]), &y1(p[1]), &dx(d[0]), &dy(d[1]);

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

void plain_skin::cap(la::vec2f p, float, float, float) {
	circle(p, 2.5);
}

void plain_skin::floor(const complex_polygon& p) {
	glColor4f(0, 0, 0, 1);
	draw(0, p);

	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2f( 1000, -1000);
	glVertex2f( 1000,  1000);
	glVertex2f(-1000,  1000);
	glVertex2f(-1000, -1000);
	glEnd();
}
