#ifndef LINEAR_ALGEBRA_MATRIX_HPP
#define LINEAR_ALGEBRA_MATRIX_HPP

#include "vec.hpp"

namespace la {

template <int Size, typename T>
struct sq_matrix {
	typedef T value_type;
	enum {
		rows = Size,
		cols = Size,
		size = Size
	};

	T v[rows * cols];
	T* operator[] (int row) { return v + row * cols; }
	T const* operator[] (int row) const { return v + row * cols; }

	// Create an uninitialized matrix
	sq_matrix();

	// Sets all values to value
	explicit sq_matrix(T value);

	sq_matrix<Size, T> transposed() const;
};

template <int Size, typename T>
sq_matrix<Size, T> operator * (const sq_matrix<Size, T>&, const sq_matrix<Size, T>&);

template <int Size, typename T>
vec<Size, T> operator * (const sq_matrix<Size, T>&, const vec<Size, T>&);

typedef sq_matrix<3, float> matrix33f;
typedef sq_matrix<4, float> matrix44f;

}

#endif
