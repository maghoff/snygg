#include <ymse/rect.hpp>
#include "segment_sequence.hpp"
#include "contour_segment.hpp"
#include "reverse_contour_segment.hpp"
#include "direction_decider.hpp"

direction_decider::direction_decider(
	boost::ptr_list<segment>& undecided_segments_,
	segment_sequence& decided_segments_
) :
	undecided_segments(undecided_segments_),
	decided_segments(decided_segments_)
{
}

direction_decider::~direction_decider() {
}

void direction_decider::decide(
	boost::ptr_list<segment>::iterator which,
	bool reverse
) {
	boost::ptr_list<segment>::auto_type seg(undecided_segments.release(which));

	std::auto_ptr<segment> contour;
	if (reverse) contour.reset(new reverse_contour_segment(seg.release()));
	else contour.reset(new contour_segment(seg.release()));

	decided_segments.push_back(contour);
}

void direction_decider::decide_some_segments() {
	ymse::rectf bb = undecided_segments.front().bounding_box();
	float y = (bb.bottom() + bb.top()) / 2.f;

	decide(undecided_segments.begin(), true);
}

void direction_decider::decide_all() {
	while (!undecided_segments.empty()) {
		size_t undecided_before = undecided_segments.size();

		decide_some_segments();

		size_t undecided_after = undecided_segments.size();
		assert(undecided_before != undecided_after);
	}
}
