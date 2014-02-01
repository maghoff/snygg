#ifndef COMPLEX_POLYGON_HPP
#define COMPLEX_POLYGON_HPP

#include <vector>
#include <vec_fwd.hpp>

class complex_polygon {
public:
	std::vector<la::vec2f> points;
	std::vector<int> triangles;

	complex_polygon();
	complex_polygon(
		std::vector<la::vec2f> points,
		std::vector<int> triangles
	);
	~complex_polygon();

	void draw() const;
	void draw_outlines() const;
};

#endif // COMPLEX_POLYGON_HPP
