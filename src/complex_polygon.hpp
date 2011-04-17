#ifndef COMPLEX_POLYGON_HPP
#define COMPLEX_POLYGON_HPP

#include <vector>

namespace ymse {
	template <int Dim, typename T>
	class vec;

	typedef vec<2, float> vec2f;
}

class complex_polygon {
	std::vector<ymse::vec2f> points;
	std::vector<int> triangles;

public:
	complex_polygon();
	complex_polygon(
		std::vector<ymse::vec2f> points,
		std::vector<int> triangles
	);
	~complex_polygon();

	void draw() const;
	void draw_outlines() const;
};

#endif // COMPLEX_POLYGON_HPP
