#include <ymse/gl.h>
#include <ymse/vec.hpp>
#include "complex_polygon.hpp"

complex_polygon::complex_polygon() { }

complex_polygon::complex_polygon(
	std::vector<ymse::vec2f> points_,
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
