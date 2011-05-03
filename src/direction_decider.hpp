#ifndef DIRECTION_DECIDER_HPP
#define DIRECTION_DECIDER_HPP

#include <boost/ptr_container/ptr_list.hpp>

class segment;
class segment_sequence;

class direction_decider {
	boost::ptr_list<segment>& undecided_segments;
	segment_sequence& decided_segments;

	void decide(boost::ptr_list<segment>::iterator which, bool reverse);
	void decide_some_segments();

public:
	direction_decider(
		boost::ptr_list<segment>& undecided_segments,
		segment_sequence& decided_segments
	);
	~direction_decider();

	void decide_all();
};

#endif // DIRECTION_DECIDER_HPP
