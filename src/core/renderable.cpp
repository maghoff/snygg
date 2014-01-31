#include <vector>
#include <deque>
#include "../segment/segment.hpp"
#include "renderable.hpp"

renderable::~renderable() {
}

template <class T>
void render_sequence(const T& seq, skin& s, float head_b) {
	for (auto i = seq.rbegin(), end = seq.rend(); i != end; ++i) {
		(*i)->render(s, head_b);
		head_b += (*i)->length();
	}
}

template void render_sequence(const std::vector<std::unique_ptr<segment>>&, skin&, float);
template void render_sequence(const std::deque<std::unique_ptr<segment>>&, skin&, float);
