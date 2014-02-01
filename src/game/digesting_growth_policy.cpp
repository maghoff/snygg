#include <cmath>
#include <vec.hpp>
#include "scored_point.hpp"
#include "segment_sequence.hpp"
#include "digesting_growth_policy.hpp"

digesting_growth_policy::digesting_growth_policy() { }
digesting_growth_policy::~digesting_growth_policy() { }

segment_ptr digesting_growth_policy::growth_segment(la::vec2f pos, la::vec2f dir, float length) {
	return segment_ptr(new scored_point(pos, sqrt(2.5*2.5 + length), 2.5f, dir));
}

void digesting_growth_policy::grow(segment_sequence& body, float length) {
	la::vec2f pos = body.get_head_pos();
	la::vec2f v_dir = body.get_head_direction();

	const float min_r = 2.5;
	float r = sqrt(min_r * min_r + length);
	body.push_back(segment_ptr(new scored_point(pos, r, min_r, v_dir)));
}
