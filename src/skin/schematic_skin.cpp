#include <algorithm>
#include <cmath>
#include <GL/glew.h>

#include <rect.hpp>
#include <vec.hpp>
#include "complex_polygon.hpp"
#include "schematic_skin.hpp"

namespace {
	const float ARROW_LENGTH = 4.;
	const float ARROW_WIDTH = 1.2;
}

schematic_skin::schematic_skin() { }
schematic_skin::~schematic_skin() { }

void schematic_skin::circle(la::vec2f p, float r) {
	float step_size = get_step_size(r);

	glColor4f(1, 1, 1, 1);

	glBegin(GL_LINE_LOOP);
	for (float d = 0.f; d < M_PI * 2.f; d += step_size) {
		glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));
	}
	glEnd();

	glColor4f(1, 1, 1, 0.3);
	plain_skin::circle(p, r);
}

void schematic_skin::blood(la::vec2f p, float r) {
	float step_size = get_step_size(r);

	glColor4f(1, 0, 0, 1);

	glBegin(GL_LINE_LOOP);
	for (float d = 0.f; d < M_PI * 2.f; d += step_size) {
		glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));
	}
	glEnd();

	glColor4f(1, 0, 0, 0.3);
	plain_skin::circle(p, r);
}

void schematic_skin::fat_arc(la::vec2f p, float r, float t, float begin_in, float end_in, float b_begin, float b_end) {
	float r1 = r-t, r2 = r+t;
	float step_size = get_step_size(r2);

	float begin = begin_in, end = end_in;
	if (begin > end) std::swap(begin, end);

	float &x(p[0]), &y(p[1]);

	glColor4f(0.7, 1.0, 0.7, 1.0);

	glBegin(GL_LINE_STRIP);

	for (float d = begin; d < end; d += step_size) {
		glVertex2f(x + r1 * cos(d), y + r1 * sin(d));
	}
	glVertex2f(x + r1 * cos(end), y + r1 * sin(end));

	glEnd();

	glBegin(GL_LINE_STRIP);

	for (float d = begin; d < end; d += step_size) {
		glVertex2f(x + r2 * cos(d), y + r2 * sin(d));
	}
	glVertex2f(x + r2 * cos(end), y + r2 * sin(end));

	glEnd();

	glBegin(GL_LINES);
	glVertex2f(x + r1 * cos(begin), y + r1 * sin(begin));
	glVertex2f(x + r2 * cos(begin), y + r2 * sin(begin));
	glVertex2f(x + r1 * cos(end), y + r1 * sin(end));
	glVertex2f(x + r2 * cos(end), y + r2 * sin(end));
	glEnd();

	if ((end - begin) * r > ARROW_LENGTH * 1.25) {
		const float dir = begin_in < end_in ? 1. : -1.;
		const float hrl = dir * (ARROW_LENGTH / 2. / r);
		const float center = (begin + end) / 2.;
		const float arrow_begin = center - hrl;
		const float arrow_end = center + hrl;
		const float arrowhead_end = center + hrl - dir * ARROW_WIDTH / r;

		glBegin(GL_LINE_STRIP);
		float abegin = arrow_begin, aend = arrow_end;
		if (abegin > aend) std::swap(abegin, aend);
		for (float d = abegin; d < aend; d += step_size) {
			glVertex2f(x + r * cos(d), y + r * sin(d));
		}
		glVertex2f(x + r * cos(aend), y + r * sin(aend));
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(x + r * cos(arrow_end), y + r * sin(arrow_end));
		glVertex2f(x + (r - ARROW_WIDTH) * cos(arrowhead_end), y + (r - ARROW_WIDTH) * sin(arrowhead_end));

		glVertex2f(x + r * cos(arrow_end), y + r * sin(arrow_end));
		glVertex2f(x + (r + ARROW_WIDTH) * cos(arrowhead_end), y + (r + ARROW_WIDTH) * sin(arrowhead_end));
		glEnd();
	}

	glColor4f(0.7, 1.0, 0.7, 0.3);
	plain_skin::fat_arc(p, r, t, begin, end, b_begin, b_end);
}

inline void glyVertex(const la::vec2f& v) {
	glVertex2f(v.x(), v.y());
}

void schematic_skin::fat_line(la::vec2f p, la::vec2f d, float len, float t, float b_begin, float b_end) {
	float &x1(p[0]), &y1(p[1]), &dx(d[0]), &dy(d[1]);

	// Calculate normal * thickness:
	float nx = dy * t, ny = -dx * t;
	float x2 = x1 + dx*len, y2 = y1 + dy * len;

	glColor4f(0.7, 0.7, 1.0, 1.0);

	glBegin(GL_LINE_LOOP);
	glVertex2f(x1 + nx, y1 + ny);
	glVertex2f(x2 + nx, y2 + ny);
	glVertex2f(x2 - nx, y2 - ny);
	glVertex2f(x1 - nx, y1 - ny);
	glEnd();

	if (len > ARROW_LENGTH * 1.25) {
		const float hll = ARROW_LENGTH / 2.;
		const la::vec2f center = p + d * (len / 2.f);
		const la::vec2f long_normal = la::vec2f(nx, ny);
		const la::vec2f normal = long_normal * (1.f / long_normal.length());

		glBegin(GL_LINES);
		glyVertex(center - hll * d);
		glyVertex(center + hll * d);

		glyVertex(center + hll * d);
		glyVertex(center + (hll - ARROW_WIDTH) * d + ARROW_WIDTH * normal);

		glyVertex(center + hll * d);
		glyVertex(center + (hll - ARROW_WIDTH) * d - ARROW_WIDTH * normal);
		glEnd();
	}

	glColor4f(0.7, 0.7, 1.0, 0.3);
	plain_skin::fat_line(p, d, len, t, b_begin, b_end);
}

static void cap_outline(la::vec2f p, float r, float snake_dir, float cap_dir, float step_size) {
	for (float d = cap_dir; d < cap_dir + M_PI; d += step_size) {
		glVertex2f(p[0] + r * cos(snake_dir + d), p[1] + r * sin(snake_dir + d));
	}

	float d = cap_dir + M_PI;
	glVertex2f(p[0] + r * cos(snake_dir + d), p[1] + r * sin(snake_dir + d));
}

void schematic_skin::cap(la::vec2f p, float snake_direction_in, float cap_direction_in, float) {
	const float r = 2.5;
	const float step_size = get_step_size(r);

	float snake_dir = snake_direction_in - M_PI*0.5;
	float cap_dir = cap_direction_in - M_PI*0.5;

	glColor4f(0.7, 1.0, 1.0, 1.0);
	glBegin(GL_LINE_LOOP);
	cap_outline(p, r, snake_dir, cap_dir, step_size);
	glEnd();

	glColor4f(0.7, 1.0, 1.0, 0.3);
	glBegin(GL_TRIANGLE_FAN);
	cap_outline(p, r, snake_dir, cap_dir, step_size);
	glEnd();
}

void schematic_skin::floor(const complex_polygon& floor_poly) {
	glColor4f(0.62, 0.62, 0.62, 1.0);
	floor_poly.draw_outlines();
	glColor4f(0.62, 0.62, 0.62, 0.3);
	floor_poly.draw();
}
