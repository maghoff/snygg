#include <GL/gl.h>
#include "snygg.hpp"

snygg::snygg() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glPointSize(20.0);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
	glLineWidth(2.5f);
}

snygg::~snygg() {
}

void snygg::render() {
	glClear(GL_COLOR_BUFFER_BIT);

	glColor4f(0.0, 0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-1, -1);
	glVertex2f(-1, 1);
	glVertex2f(1, 1);
	glVertex2f(1, -1);
	glEnd();
}

void snygg::tick() {
}
