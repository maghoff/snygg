#include <algorithm>
#include <cmath>
#include <ymse/gl.h>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "plain_skin.hpp"

plain_skin::plain_skin() {
}

void plain_skin::circle(ymse::vec2f p, float r) {
	float step_size = get_step_size(r);

	glBegin(GL_TRIANGLE_FAN);
	for (float d = 0.f; d < M_PI * 2.f; d += step_size) {
		glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));
	}
	glEnd();
}

void plain_skin::blood(ymse::vec2f p, float r) {
	glColor4f(1, 0, 0, 1);
	circle(p, r);
	glColor4f(1, 1, 1, 1);
}

void plain_skin::fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
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

void plain_skin::fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end) {
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

void plain_skin::cap(ymse::vec2f p, float, float, float) {
	circle(p, 2.5);
}

void plain_skin::finish_frame(ymse::rectf) {
}
