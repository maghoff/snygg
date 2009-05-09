#include <cmath>
#include <GL/gl.h>
#include "plain_skin.hpp"
#include "snygg.hpp"

snygg::snygg() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);

	glDisable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	active_skin.reset(new plain_skin);
}

snygg::~snygg() {
}


void box(skin& s, float x1, float y1, float w, float h, float r) {
	float x2 = x1 + w, y2 = y1 + h;

	s.fat_line(x1 + r, y1    ,  1,  0, w - 2*r, 2.5);
	s.fat_arc (x2 - r, y1 + r,  r, 2.5,  M_PI * -0.5, M_PI *  0.0);
	s.fat_line(x2    , y1 + r,  0,  1, h - 2*r, 2.5);
	s.fat_arc (x2 - r, y2 - r,  r, 2.5,  M_PI *  0.0, M_PI *  0.5);
	s.fat_line(x2 - r, y2    , -1,  0, w - 2*r, 2.5);
	s.fat_arc (x1 + r, y2 - r,  r, 2.5,  M_PI *  0.5, M_PI *  1.0);
	s.fat_line(x1    , y2 - r,  0, -1, h - 2*r, 2.5);
	s.fat_arc (x1 + r, y1 + r,  r, 2.5,  M_PI * -1.0, M_PI * -0.5);
}


void snygg::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	box(*active_skin, -200, -50, 400, 100, 10);
}

void snygg::tick() {
}
