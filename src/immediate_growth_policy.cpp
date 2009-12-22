#include <ymse/vec.hpp>
#include "extender.hpp"
#include "segment_sequence.hpp"
#include "immediate_growth_policy.hpp"

immediate_growth_policy::immediate_growth_policy() { }
immediate_growth_policy::~immediate_growth_policy() { }

segment_ptr immediate_growth_policy::growth_segment(ymse::vec2f pos, ymse::vec2f dir, float length) {
	return segment_ptr(new extender(pos, dir, length));
}

void immediate_growth_policy::grow(segment_sequence& body, float length) {
	ymse::vec2f pos = body.get_tail_pos();
	ymse::vec2f dir(1, 0); //< This direction is never read from the extender
	body.push_front(segment_ptr(new extender(pos, dir, length)));
}
