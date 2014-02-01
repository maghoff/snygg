#ifndef SNYGG_INTERSECTABLE_WITH_CIRCLE_HPP
#define SNYGG_INTERSECTABLE_WITH_CIRCLE_HPP

#include <vec_fwd.hpp>

class intersectable_with_circle {
public:
	virtual ~intersectable_with_circle();

	virtual bool intersect_with_circle(const la::vec2f&, float r) const = 0;
	virtual bool intersect_with_circle(const la::vec2f&, float r, float& skiplength) const;
};

#endif
