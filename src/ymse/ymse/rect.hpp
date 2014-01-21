#ifndef YMSE_RECT_HPP
#define YMSE_RECT_HPP

namespace ymse {

template <typename T>
struct rect {
	typedef T value_type;

	value_type x1, y1, x2, y2;

	value_type top() const { return y1; }
	value_type left() const { return x1; }

	value_type bottom() const { return y2; }
	value_type right() const { return x2; }

	value_type width() const { return x2-x1; }
	value_type height() const { return y2-y1; }
};

typedef rect<float> rectf;

template <typename T>
rect<T> bounding_box(rect<T>, rect<T>);

template <int Dim, typename T>
class vec;

template <typename T>
rect<T> bounding_box(rect<T>, vec<2, T>);

}

#endif // YMSE_RECT_HPP
