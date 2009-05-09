#include <iostream>
#include <algorithm>
#include <cmath>
#include <GL/gl.h>
#include "plain_skin.hpp"

void plain_skin::fat_arc(float x, float y, float r, float t, float begin, float end) {
	float r1 = r-t, r2 = r+t;
	if (begin > end) std::swap(begin, end);

	glBegin(GL_TRIANGLE_STRIP);

	for (float d = begin; d < end; d += 0.1f) {
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
