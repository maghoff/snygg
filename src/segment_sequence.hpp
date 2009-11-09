#ifndef SNYGG_SEGMENT_SEQUENCE_HPP
#define SNYGG_SEGMENT_SEQUENCE_HPP

#include <memory>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "segment.hpp"

typedef std::auto_ptr<segment> segment_ptr;

class segment_sequence : public boost::noncopyable, public segment {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	segment_sequence();
	~segment_sequence();

	void head_forward(float);
	float tail_forward(float);

	bool intersect_with_circle(float x, float y, float r) const;

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_tail_pos() const;
	ymse::vec2f get_head_direction() const;

	void render(skin&) const;

	// This segment_sequence assumes ownership
	void push_back(std::auto_ptr<segment>);
};

#endif
