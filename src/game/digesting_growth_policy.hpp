#ifndef SNYGG_DIGESTING_GROWTH_POLICY_HPP
#define SNYGG_DIGESTING_GROWTH_POLICY_HPP

#include "growth_policy.hpp"

class digesting_growth_policy : public growth_policy {
public:
	digesting_growth_policy();
	~digesting_growth_policy();

	segment_ptr growth_segment(ymse::vec2f pos, ymse::vec2f dir, float length);
	void grow(segment_sequence& body, float length);
};

#endif // SNYGG_DIGESTING_GROWTH_POLICY_HPP
