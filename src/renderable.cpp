#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include "segment.hpp"
#include "renderable.hpp"

renderable::~renderable() {
}

template <class T>
void render_sequence(const T& seq, skin& s) {
	typedef typename T::const_iterator it;
	it end = seq.end();
	for (it i = seq.begin(); i != end; ++i) i->render(s);
}

template void render_sequence(const boost::ptr_vector<segment>&, skin&);
template void render_sequence(const boost::ptr_list<segment>&, skin&);
