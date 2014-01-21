#ifndef YMSE_VEC_HPP
#define YMSE_VEC_HPP

// For typedefs:
#include "vec_fwd.hpp"

namespace ymse {

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

	vec();
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
ymse::vec<Dim, T> operator + (const ymse::vec<Dim, T>&, const ymse::vec<Dim, T>&);

template <int Dim, typename T>
ymse::vec<Dim, T> operator - (const ymse::vec<Dim, T>&, const ymse::vec<Dim, T>&);

// element-wise division
template <int Dim, typename T>
ymse::vec<Dim, T> operator / (const ymse::vec<Dim, T>&, const ymse::vec<Dim, T>&);

template <int Dim, typename T>
ymse::vec<Dim, T> operator * (const ymse::vec<Dim, T>&, T);

template <int Dim, typename T>
ymse::vec<Dim, T> operator * (T, const ymse::vec<Dim, T>&);

template <int Dim, typename T>
bool operator == (const ymse::vec<Dim, T>&, const ymse::vec<Dim, T>&);

template <int Dim, typename T>
bool operator != (const ymse::vec<Dim, T>&, const ymse::vec<Dim, T>&);

// Lexicographic order
template <int Dim, typename T>
bool operator < (const ymse::vec<Dim, T>&, const ymse::vec<Dim, T>&);

}

#endif
