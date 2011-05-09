#include <cassert>
#include <stdexcept>
#include <boost/scoped_array.hpp>
#include "complex_polygon.hpp"
#include "complex_polygon_triangulator_triangulation.hpp"

extern "C" {
#include "triangulation/interface.h"
}


// How far apart to points must be to not be considered the same point:
const double EPSILON = 0.01;


complex_polygon_triangulator_triangulation::complex_polygon_triangulator_triangulation() :
	state(initial)
{
}

complex_polygon_triangulator_triangulation::~complex_polygon_triangulator_triangulation() { }

unsigned complex_polygon_triangulator_triangulation::current_contour_size() const {
	assert(state == in_contour);
	return points.size() - contour_start;
}

void complex_polygon_triangulator_triangulation::start_contour() {
	assert(state == initial);
	state = in_contour;

	contour_start = points.size();
	contour_is_reverse = false;
}

void complex_polygon_triangulator_triangulation::start_reverse_contour() {
	assert(state == initial);
	state = in_contour;

	contour_start = points.size();
	contour_is_reverse = true;
}

void complex_polygon_triangulator_triangulation::end_contour() {
	assert(state == in_contour);

	unsigned size = current_contour_size();
	if (size != 0) contour_sizes.push_back(size);

	state = initial;
}

void complex_polygon_triangulator_triangulation::point(ymse::vec2f p) {
	assert(state == in_contour);

	if (current_contour_size() != 0) {
		const double EPSILON_SQ = EPSILON * EPSILON;
		if ((points.back() - p).square_length() < EPSILON_SQ) return;
		if ((points[contour_start] - p).square_length() < EPSILON_SQ) return;
	}
	if (contour_is_reverse) {
		points.insert(points.begin() + contour_start, p);
	} else {
		points.push_back(p);
	}
}

complex_polygon complex_polygon_triangulator_triangulation::get_complex_polygon() const {
	assert(state == initial);

	boost::scoped_array<double[2]> pts(new double[points.size()+1][2]);
	for (size_t i=0; i<points.size(); ++i) {
		pts[i+1][0] = points[i][0];
		pts[i+1][1] = points[i][1];
	}

	const unsigned n_triangles = points.size() - 4 + 2 * contour_sizes.size();
	boost::scoped_array<int[3]> tris(new int[n_triangles][3]);

	int res = triangulate_polygon(
		contour_sizes.size(), const_cast<int*>(contour_sizes.data()),
		pts.get(),
		tris.get()
	);

	if (res != 0) {
		throw std::runtime_error("complex_polygon_triangulator_triangulation::get_complex_polygon(): triangulate_polygon failed");
	}

	std::vector<int> triangles(3 * n_triangles);
	for (unsigned i=0; i<n_triangles; ++i) {
		triangles[i*3 + 0] = tris[i][0] - 1;
		triangles[i*3 + 1] = tris[i][1] - 1;
		triangles[i*3 + 2] = tris[i][2] - 1;
	}

	return complex_polygon(points, triangles);
}
