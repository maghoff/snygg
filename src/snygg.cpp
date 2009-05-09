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

void snygg::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	active_skin->fat_arc(0, 0, 50, 10, 0, M_PI);
	active_skin->fat_line(-50, 0, 0, 1, -50, 10);
	active_skin->fat_line(50, 0, 0, 1, -50, 10);
	active_skin->fat_arc(0, -50, 50, 10, M_PI, 2.0*M_PI);
}

void snygg::tick() {
}
