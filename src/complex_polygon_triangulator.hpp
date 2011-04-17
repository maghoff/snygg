#ifndef COMPLEX_POLYGON_TRIANGULATOR_HPP
#define COMPLEX_POLYGON_TRIANGULATOR_HPP

#include <vector>
#include <ymse/vec.hpp>

class complex_polygon;

class complex_polygon_triangulator {
	std::vector<int> contour_sizes;
	std::vector<ymse::vec2f> points;

	int contour_start;

	enum { initial, in_contour } state;

	unsigned current_contour_size() const;

public:
	complex_polygon_triangulator();
	~complex_polygon_triangulator();

	void start_contour();
	void end_contour();

	void point(ymse::vec2f);

	complex_polygon get_complex_polygon() const;
};

#endif // COMPLEX_POLYGON_TRIANGULATOR_HPP
