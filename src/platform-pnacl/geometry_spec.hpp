#ifndef GEOMETRY_SPEC_HPP
#define GEOMETRY_SPEC_HPP

#include <vector>
#include "vertex_spec.hpp"
#include "geometry_mode.hpp"

struct geometry_spec {
	geometry_mode mode;
	std::vector<vertex_spec> data;
};

#endif
