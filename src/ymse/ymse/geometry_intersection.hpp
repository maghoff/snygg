#ifndef YMSE_GEOMETRY_INTERSECTION_HPP
#define YMSE_GEOMETRY_INTERSECTION_HPP

#include <vec_fwd.hpp>

namespace ymse {
namespace intersect {

bool circle_with_circle(la::vec2f p1, float r1, la::vec2f p2, float r2);

}}

#endif // YMSE_GEOMETRY_INTERSECTION_HPP
