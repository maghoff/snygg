#ifndef LINEAR_ALGEBRA_VEC_HPP
#define LINEAR_ALGEBRA_VEC_HPP

// For typedefs:
#include "vec_fwd.hpp"

namespace la {

template <int Dim, typename T>
struct vec {
	typedef T value_type;

	T v[Dim];
	T operator[] (int n) const { return v[n]; }
	T& operator[] (int n) { return v[n]; }

	T& x();
	T& y();
	T& z();

	T x() const;
	T y() const;
	T z() const;

	vec() = default;
	vec(T x, T y);
	vec(T x, T y, T z);
	vec(T x, T y, T z, T w);

	vec<Dim, T>& operator += (vec<Dim, T>);
	vec<Dim, T>& operator -= (vec<Dim, T>);
	vec<Dim, T>& operator /= (vec<Dim, T>);

	vec<Dim, T>& operator *= (T);

	T length() const;
	T square_length() const;
};

template <int Dim, typename T>
la::vec<Dim, T> operator + (const la::vec<Dim, T>&, const la::vec<Dim, T>&);

template <int Dim, typename T>
la::vec<Dim, T> operator - (const la::vec<Dim, T>&, const la::vec<Dim, T>&);

// element-wise division
template <int Dim, typename T>
la::vec<Dim, T> operator / (const la::vec<Dim, T>&, const la::vec<Dim, T>&);

template <int Dim, typename T>
la::vec<Dim, T> operator * (const la::vec<Dim, T>&, T);

template <int Dim, typename T>
la::vec<Dim, T> operator * (T, const la::vec<Dim, T>&);

template <int Dim, typename T>
bool operator == (const la::vec<Dim, T>&, const la::vec<Dim, T>&);

template <int Dim, typename T>
bool operator != (const la::vec<Dim, T>&, const la::vec<Dim, T>&);

// Lexicographic order
template <int Dim, typename T>
bool operator < (const la::vec<Dim, T>&, const la::vec<Dim, T>&);

}

#endif
