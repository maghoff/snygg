#ifndef SEGMENT_FILTER_HPP
#define SEGMENT_FILTER_HPP

#include <boost/scoped_ptr.hpp>
#include "segment.hpp"

class segment_filter : public segment {
	boost::scoped_ptr<segment> s;

public:
	segment_filter(segment_ptr);
	~segment_filter();

	void head_forward(float);
	float tail_forward(float);

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_head_direction() const;
	ymse::vec2f get_tail_pos() const;
	ymse::vec2f get_tail_direction() const;

	float length() const;
	void render(skin&, float head_b) const;

	ymse::rectf bounding_box() const;

	int left_hline_intersections(ymse::vec2f p) const;

	void add_to_triangulator(complex_polygon_triangulator*) const;

	bool intersect_with_circle(const ymse::vec2f&, float r) const;
};

#endif // SEGMENT_FILTER_HPP
