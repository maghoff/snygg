#ifndef COMPLEX_POLYGON_HPP
#define COMPLEX_POLYGON_HPP

#include <vector>
#include <vec_fwd.hpp>

struct complex_polygon {
	std::vector<la::vec2f> points;
	std::vector<int> triangles;
};

#endif // COMPLEX_POLYGON_HPP
