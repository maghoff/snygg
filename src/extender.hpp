#ifndef SNYGG_EXTENDER_HPP
#define SNYGG_EXTENDER_HPP

#include <boost/scoped_ptr.hpp>
#include "segment.hpp"

class extender : public segment {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	extender(
		ymse::vec2f pos,
		ymse::vec2f dir,
		float length
	);
	~extender();

	void head_forward(float);
	float tail_forward(float);

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_tail_pos() const;
	ymse::vec2f get_head_direction() const;

	float length() const;
	void render(skin&, float head_b) const;

	ymse::rectf bounding_box() const;

	int left_hline_intersections(ymse::vec2f p) const;

	bool intersect_with_circle(const ymse::vec2f&, float r) const;
	bool intersect_with_circle(const ymse::vec2f&, float r, float& skiplength) const;
};

#endif // SNYGG_EXTENDER_HPP
