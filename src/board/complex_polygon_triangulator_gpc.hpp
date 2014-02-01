#ifndef COMPLEX_POLYGON_TRIANGULATOR_GPC_HPP
#define COMPLEX_POLYGON_TRIANGULATOR_GPC_HPP

#include <vector>
#include "complex_polygon_triangulator.hpp"

extern "C" {
#include "gpc/gpc.h"
}

class complex_polygon_triangulator_gpc : public complex_polygon_triangulator {
	std::vector<gpc_vertex> current_contour;
	gpc_polygon total_polygon;

public:
	complex_polygon_triangulator_gpc();
	~complex_polygon_triangulator_gpc();

	void start_contour();
	void end_contour();

	void point(la::vec2f);

	complex_polygon get_complex_polygon() const;
};

#endif // COMPLEX_POLYGON_TRIANGULATOR_GPC_HPP
