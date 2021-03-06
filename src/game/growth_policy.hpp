#ifndef SNYGG_GROWTH_POLICY_HPP
#define SNYGG_GROWTH_POLICY_HPP

#include <memory>
#include <vec_fwd.hpp>

class segment;
class segment_sequence;

class growth_policy {
public:
	growth_policy();
	virtual ~growth_policy();

	virtual std::unique_ptr<segment> growth_segment(la::vec2f pos, la::vec2f dir, float length) = 0;
	virtual void grow(segment_sequence& body, float length) = 0;
};

#endif // SNYGG_GROWTH_POLICY_HPP
