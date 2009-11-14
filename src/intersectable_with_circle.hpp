#ifndef SNYGG_INTERSECTABLE_WITH_CIRCLE_HPP
#define SNYGG_INTERSECTABLE_WITH_CIRCLE_HPP

namespace ymse {
	template <int dim, typename T> struct vec;
	typedef vec<2, float> vec2f;
}

class intersectable_with_circle {
public:
	virtual ~intersectable_with_circle();

	virtual bool intersect_with_circle(const ymse::vec2f&, float r) const = 0;
};

#endif
