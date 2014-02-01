#include <ymse/gl.h>
#include <vec.hpp>
#include "complex_polygon.hpp"

complex_polygon::complex_polygon() { }

complex_polygon::complex_polygon(
	std::vector<la::vec2f> points_,
	std::vector<int> triangles_
) :
	points(points_),
	triangles(triangles_)
{
}

complex_polygon::~complex_polygon() {
}

void complex_polygon::draw() const {
	glBegin(GL_TRIANGLES);
	typedef std::vector<int>::const_iterator it;
	for (it i = triangles.begin(); i != triangles.end(); ++i) {
		glVertex2f(points[*i].x(), points[*i].y());
	}
	glEnd();
}

void complex_polygon::draw_outlines() const {
	for (unsigned i=0; i<triangles.size(); i += 3) {
		glBegin(GL_LINE_LOOP);
		glVertex2f(points[triangles[i+0]].x(), points[triangles[i+0]].y());
		glVertex2f(points[triangles[i+1]].x(), points[triangles[i+1]].y());
		glVertex2f(points[triangles[i+2]].x(), points[triangles[i+2]].y());
		glEnd();
	}
}
