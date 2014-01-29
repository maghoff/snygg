#ifndef SNYGG_SEGMENT_SEQUENCE_HPP
#define SNYGG_SEGMENT_SEQUENCE_HPP

#include <memory>
#include <boost/noncopyable.hpp>
#include "segment.hpp"

class segment_sequence : public boost::noncopyable, public segment {
	struct impl;
	std::unique_ptr<impl> d;

public:
	segment_sequence();
	~segment_sequence();

	void head_forward(float) override;
	float tail_forward(float) override;

	bool intersect_with_circle(const ymse::vec2f&, float r) const override;
	bool intersect_with_self(const ymse::vec2f& p, float r) const;

	ymse::vec2f get_head_pos() const override;
	ymse::vec2f get_head_direction() const override;
	ymse::vec2f get_tail_pos() const override;
	ymse::vec2f get_tail_direction() const override;

	float length() const override;
	void render(skin&, float head_b) const override;

	ymse::rectf bounding_box() const override;

	int left_hline_intersections(ymse::vec2f) const override;

	void add_to_triangulator(complex_polygon_triangulator&) const override;

	void push_back(std::unique_ptr<segment>&&);
	void push_front(std::unique_ptr<segment>&&);

	// Added for convenience for luabind
	void push_back(segment*);
};

#endif
