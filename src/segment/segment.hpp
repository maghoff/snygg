#ifndef SNYGG_SEGMENT_HPP
#define SNYGG_SEGMENT_HPP

#include <memory>
#include "intersectable_with_circle.hpp"
#include "renderable.hpp"

namespace ymse {
	template <typename T>
	struct rect;

	typedef rect<float> rectf;
}

class complex_polygon_triangulator;
class skin;

class segment : /*public renderable,*/ public intersectable_with_circle {
public:
	virtual void head_forward(float) = 0;
	virtual float tail_forward(float) = 0;

	virtual la::vec2f get_head_pos() const = 0;
	virtual la::vec2f get_head_direction() const = 0;
	virtual la::vec2f get_tail_pos() const = 0;
	virtual la::vec2f get_tail_direction() const = 0;

	virtual float length() const = 0;
	virtual void render(skin&, float head_b) const = 0;

	virtual ymse::rectf bounding_box() const = 0;

	// How many times does the line defined by
	//    y = p[1]
	// intersect with this segment at
	//    x < p[0]
	//
	// This is used to determine the winding number of a new food item.
	virtual int left_hline_intersections(la::vec2f p) const = 0;

	virtual void add_to_triangulator(complex_polygon_triangulator&) const { }
};

typedef std::unique_ptr<segment> segment_ptr;

#endif
