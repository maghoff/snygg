#include <vec.hpp>
#include "extender.hpp"
#include "segment_sequence.hpp"
#include "immediate_growth_policy.hpp"

immediate_growth_policy::immediate_growth_policy() { }
immediate_growth_policy::~immediate_growth_policy() { }

segment_ptr immediate_growth_policy::growth_segment(la::vec2f pos, la::vec2f dir, float length) {
	return segment_ptr(new extender(pos, dir, length));
}

void immediate_growth_policy::grow(segment_sequence& body, float length) {
	la::vec2f pos = body.get_tail_pos();
	la::vec2f dir = body.get_tail_direction();
	body.push_front(segment_ptr(new extender(pos, dir, length)));
}
