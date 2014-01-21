#include "vec.hpp"
//#include "geometry_intersection.hpp"

namespace {

float sqr(float x) { return x*x; }

}

namespace ymse {
namespace intersect {

bool circle_with_circle(vec2f p1, float r1, vec2f p2, float r2) {
	vec2f diff = p2;
	diff -= p1;

	return (sqr(diff[0]) + sqr(diff[1])) <= sqr(r1 + r2);
}

}}
