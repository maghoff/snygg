#ifndef VERTEX_SPEC
#define VERTEX_SPEC

#include <vec.hpp>

struct vertex_spec {
	la::vec2f vertex;
	la::vec3f across;
	la::vec3f along;
	la::vec2f circle_coord;
	float b;
};

#endif
