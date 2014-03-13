#ifndef GEOMETRY_SPEC_HPP
#define GEOMETRY_SPEC_HPP

#include <vector>
#include "vertex_spec.hpp"

struct geometry_spec {
	enum { triangle_strip, triangle_fan } mode;
	std::vector<vertex_spec> data;
};

#endif
