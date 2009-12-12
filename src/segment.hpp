#ifndef SNYGG_SEGMENT_HPP
#define SNYGG_SEGMENT_HPP

#include "intersectable_with_circle.hpp"
#include "renderable.hpp"

namespace ymse {
	template <int dim, typename T>
	struct vec;

	typedef vec<2, float> vec2f;

	template <typename T>
	struct rect;

	typedef rect<float> rectf;
}

class skin;

class segment : /*public renderable,*/ public intersectable_with_circle {
public:
	virtual void head_forward(float) = 0;
	virtual float tail_forward(float) = 0;

	virtual ymse::vec2f get_head_pos() const = 0;
	virtual ymse::vec2f get_tail_pos() const = 0;
	virtual ymse::vec2f get_head_direction() const = 0;

	virtual float length() const = 0;
	virtual void render(skin&, float head_b) const = 0;

	virtual ymse::rectf bounding_box() const = 0;
};

#endif
