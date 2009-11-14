#include "intersectable_with_circle.hpp"

intersectable_with_circle::~intersectable_with_circle() {
}

bool intersectable_with_circle::intersect_with_circle(const ymse::vec2f& p, float r, float& skiplength) const {
	return intersect_with_circle(p, r);
}
