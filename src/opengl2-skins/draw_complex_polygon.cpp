#include "draw_complex_polygon.hpp"
#include <GL/glew.h>
#include <vec.hpp>
#include "complex_polygon.hpp"

void draw(const complex_polygon& polygon) {
	glBegin(GL_TRIANGLES);
	for (auto i : polygon.triangles) {
		glVertex2f(polygon.points[i].x(), polygon.points[i].y());
	}
	glEnd();
}

void draw_outlines(const complex_polygon& polygon) {
	auto& points = polygon.points;
	auto& triangles = polygon.triangles;

	for (unsigned i=0; i<triangles.size(); i += 3) {
		glBegin(GL_LINE_LOOP);
		glVertex2f(points[triangles[i+0]].x(), points[triangles[i+0]].y());
		glVertex2f(points[triangles[i+1]].x(), points[triangles[i+1]].y());
		glVertex2f(points[triangles[i+2]].x(), points[triangles[i+2]].y());
		glEnd();
	}
}
