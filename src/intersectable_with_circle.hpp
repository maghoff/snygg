#ifndef SNYGG_INTERSECTABLE_WITH_CIRCLE_HPP
#define SNYGG_INTERSECTABLE_WITH_CIRCLE_HPP

class intersectable_with_circle {
public:
	virtual ~intersectable_with_circle();

	virtual bool intersect_with_circle(float x, float y, float r) const = 0;
};

#endif
