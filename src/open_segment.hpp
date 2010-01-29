#ifndef OPEN_SEGMENT_HPP
#define OPEN_SEGMENT_HPP

#include <boost/scoped_ptr.hpp>
#include "segment.hpp"

class open_segment : public segment {
	boost::scoped_ptr<segment> s;

public:
	open_segment(segment_ptr);
	open_segment(segment*); //< For luabind... hmm...
	~open_segment();

	// Most of the following functions are just forwarded to the underlying segment:

	void head_forward(float);
	float tail_forward(float);

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_head_direction() const;
	ymse::vec2f get_tail_pos() const;
	ymse::vec2f get_tail_direction() const;

	float length() const;
	void render(skin&, float head_b) const;

	ymse::rectf bounding_box() const;

	// Always returns 0, since this segment is open (it has only one side)
	int left_hline_intersections(ymse::vec2f p) const;

	bool intersect_with_circle(const ymse::vec2f&, float r) const;
};

#endif

