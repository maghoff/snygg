#ifndef GEOMETRY_BUILDER_HPP
#define GEOMETRY_BUILDER_HPP

#include <functional>
#include <vec_fwd.hpp>

struct geometry_spec;
struct complex_polygon;

namespace geometry_builder {
	geometry_spec circle(const std::function<float(float)>& get_step_size, la::vec2f p, float r);

	geometry_spec fat_arc(const std::function<float(float)>& get_step_size, la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	geometry_spec fat_line(la::vec2f p, la::vec2f d, float len, float t, float b_begin, float b_end);

	geometry_spec cap(const std::function<float(float)>& get_step_size, la::vec2f p, float snake_direction, float cap_direction, float b_coord);
};

#endif
