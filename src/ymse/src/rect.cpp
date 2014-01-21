#include <algorithm>
#include "rect.hpp"
#include "vec.hpp"

namespace ymse {

template struct rect<float>;


template<>
rectf bounding_box(rectf a, rectf b) {
	rectf r = {
		std::min(a.x1, b.x1), std::min(a.y1, b.y1),
		std::max(a.x2, b.x2), std::max(a.y2, b.y2)
	};
	return r;
}

template <typename T>
rect<T> bounding_box(rect<T> r, vec<2, T> b) {
	rect<T> bb;
	bb.x1 = std::min(r.x1, b[0]);
	bb.x2 = std::max(r.x2, b[0]);
	bb.y1 = std::min(r.y1, b[1]);
	bb.y2 = std::max(r.y2, b[1]);
	return bb;
}

template
rectf bounding_box(rectf, vec2f);

}
