#include <cassert>
#include <stdexcept>
#include <limits>
#include <vec.hpp>
#include "complex_polygon.hpp"
#include "complex_polygon_triangulator_gpc.hpp"

complex_polygon_triangulator_gpc::complex_polygon_triangulator_gpc() {
	total_polygon.num_contours = 0;
	total_polygon.hole = 0;
	total_polygon.contour = 0;
}

complex_polygon_triangulator_gpc::~complex_polygon_triangulator_gpc() { }

void complex_polygon_triangulator_gpc::start_contour() {
	assert(current_contour.empty());
}

void complex_polygon_triangulator_gpc::end_contour() {
	gpc_polygon current_polygon = {0, 0, 0};
	gpc_polygon next_polygon = {0, 0, 0};

	if (current_contour.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) throw std::runtime_error("Contour too large");
	gpc_vertex_list contour = {static_cast<int>(current_contour.size()), current_contour.data()};

	gpc_add_contour(&current_polygon, &contour, 0);

	gpc_polygon_clip(GPC_XOR, &total_polygon, &current_polygon, &next_polygon);

	gpc_free_polygon(&current_polygon);
	gpc_free_polygon(&total_polygon);
	total_polygon = next_polygon;

	current_contour.clear();
}

void complex_polygon_triangulator_gpc::point(la::vec2f p) {
	gpc_vertex v = {p.x(), p.y()};
	current_contour.push_back(v);
}

complex_polygon complex_polygon_triangulator_gpc::get_complex_polygon() const {
	std::vector<la::vec2f> v;
	std::vector<int> t;

	gpc_tristrip tristrip = {0, 0};

	try {
		gpc_polygon_to_tristrip(&const_cast<complex_polygon_triangulator_gpc*>(this)->total_polygon, &tristrip);

		for (int i=0; i<tristrip.num_strips; ++i) {
			gpc_vertex_list& l = tristrip.strip[i];
			for (int j=0; j<l.num_vertices; ++j) {
				gpc_vertex& p = l.vertex[j];
				v.push_back(la::vec2f(p.x, p.y));
				if (j >= 2) {
					t.push_back(v.size()-3);
					t.push_back(v.size()-2);
					t.push_back(v.size()-1);
				}
			}
		}
		gpc_free_tristrip(&tristrip);
	}
	catch (...) {
		gpc_free_tristrip(&tristrip);
		throw;
	}

	return complex_polygon{v, t};
}
