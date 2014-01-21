#ifndef YMSE_GEOMETRY_INTERSECTION_HPP
#define YMSE_GEOMETRY_INTERSECTION_HPP

namespace ymse {

template <int Dim, typename T>
struct vec;

typedef vec<2, float> vec2f;

namespace intersect {

bool circle_with_circle(vec2f p1, float r1, vec2f p2, float r2);

}}

#endif // YMSE_GEOMETRY_INTERSECTION_HPP
