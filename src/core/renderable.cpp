#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include "../segment/segment.hpp"
#include "renderable.hpp"

renderable::~renderable() {
}

template <class T>
void render_sequence(const T& seq, skin& s, float head_b) {
	typedef typename T::const_reverse_iterator it;
	it end = seq.rend();
	for (it i = seq.rbegin(); i != end; ++i) {
		i->render(s, head_b);
		head_b += i->length();
	}
}

template void render_sequence(const boost::ptr_vector<segment>&, skin&, float);
template void render_sequence(const boost::ptr_list<segment>&, skin&, float);
