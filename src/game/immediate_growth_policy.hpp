#ifndef SNYGG_IMMEDIATE_GROWTH_POLICY_HPP
#define SNYGG_IMMEDIATE_GROWTH_POLICY_HPP

#include "growth_policy.hpp"

class immediate_growth_policy : public growth_policy {
public:
	immediate_growth_policy();
	~immediate_growth_policy();

	segment_ptr growth_segment(la::vec2f pos, la::vec2f dir, float length);
	void grow(segment_sequence& body, float length);
};

#endif // SNYGG_IMMEDIATE_GROWTH_POLICY_HPP
