#ifndef COMPLEX_POLYGON_TRIANGULATOR_HPP
#define COMPLEX_POLYGON_TRIANGULATOR_HPP

#include <vec_fwd.hpp>

class complex_polygon;

class complex_polygon_triangulator {
public:
	virtual ~complex_polygon_triangulator();

	virtual void start_contour() = 0;
	virtual void end_contour() = 0;

	virtual void point(la::vec2f) = 0;

	virtual complex_polygon get_complex_polygon() const = 0;
};

#endif // COMPLEX_POLYGON_TRIANGULATOR_HPP
