#ifndef SNYGG_SEGMENT_HEAP_HPP
#define SNYGG_SEGMENT_HEAP_HPP

#include <memory>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <ymse/vec_fwd.hpp>

class segment;

class segment_heap : public boost::noncopyable
{
	struct impl;
	boost::scoped_ptr<impl> d;

	std::unique_ptr<segment> get_a_connected_sequence();

public:
	segment_heap();
	~segment_heap();

	void line(ymse::vec2f a, ymse::vec2f b);
	void line(float x1, float y1, float x2, float y2);

	void arc(ymse::vec2f p, float r, float begin, float end);
	void arc(float x, float y, float r, float begin, float end);

	std::unique_ptr<segment> to_segment();
};

#endif // SEGMENT_HEAP_HPP
