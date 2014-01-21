#include <cassert>
#include <cmath>
#include "vec.hpp"

namespace ymse {

template <int Dim, typename T>
vec<Dim, T>::vec() {
	assert(Dim >= 2);
}

template <int Dim, typename T>
vec<Dim, T>::vec(T x_, T y_) {
	assert(Dim == 2);
	if (Dim > 0) v[0] = x_;
	if (Dim > 1) v[1] = y_;
}

template <int Dim, typename T>
vec<Dim, T>::vec(T x_, T y_, T z_) {
	assert(Dim == 3);
	if (Dim > 0) v[0] = x_;
	if (Dim > 1) v[1] = y_;
	if (Dim > 2) v[2] = z_;
}

template <int Dim, typename T>
vec<Dim, T>::vec(T x_, T y_, T z_, T w_) {
	assert(Dim == 4);
	if (Dim > 0) v[0] = x_;
	if (Dim > 1) v[1] = y_;
	if (Dim > 2) v[2] = z_;
	if (Dim > 3) v[3] = w_;
}

template <int Dim, typename T> T& vec<Dim, T>::x() { assert(Dim >= 1); return v[0]; }
template <int Dim, typename T> T& vec<Dim, T>::y() { assert(Dim >= 2); return v[1]; }
template <int Dim, typename T> T& vec<Dim, T>::z() { assert(Dim >= 3); return v[2]; }

template <int Dim, typename T> T vec<Dim, T>::x() const { assert(Dim >= 1); return v[0]; }
template <int Dim, typename T> T vec<Dim, T>::y() const { assert(Dim >= 2); return v[1]; }
template <int Dim, typename T> T vec<Dim, T>::z() const { assert(Dim >= 3); return v[2]; }

template <int Dim, typename T>
vec<Dim, T>& vec<Dim, T>::operator += (vec<Dim, T> rhs) {
	for (int i=0; i<Dim; ++i) v[i] += rhs[i];
	return *this;
}

template <int Dim, typename T>
vec<Dim, T>& vec<Dim, T>::operator -= (vec<Dim, T> rhs) {
	for (int i=0; i<Dim; ++i) v[i] -= rhs[i];
	return *this;
}

template <int Dim, typename T>
vec<Dim, T>& vec<Dim, T>::operator /= (vec<Dim, T> rhs) {
	for (int i=0; i<Dim; ++i) v[i] /= rhs[i];
	return *this;
}

template <int Dim, typename T>
vec<Dim, T>& vec<Dim, T>::operator *= (T rhs) {
	for (int i=0; i<Dim; ++i) v[i] *= rhs;
	return *this;
}

template <int Dim, typename T>
T vec<Dim, T>::length() const {
	return sqrt(square_length());
}

template <>
int vec<2, int>::length() const {
	return sqrt((double)square_length());
}

template <int Dim, typename T>
T vec<Dim, T>::square_length() const {
	T acc(0);
	for (int i=0; i<Dim; ++i) acc += v[i] * v[i];
	return acc;
}

template <int Dim, typename T>
vec<Dim, T> operator + (const vec<Dim, T>& lhs, const vec<Dim, T>& rhs) {
	vec<Dim, T> r(lhs);
	r += rhs;
	return r;
}

template <int Dim, typename T>
vec<Dim, T> operator - (const vec<Dim, T>& lhs, const vec<Dim, T>& rhs) {
	vec<Dim, T> r(lhs);
	r -= rhs;
	return r;
}

template <int Dim, typename T>
vec<Dim, T> operator / (const ymse::vec<Dim, T>& lhs, const ymse::vec<Dim, T>& rhs)
{
	vec<Dim, T> r(lhs);
	r /= rhs;
	return r;
}

template <int Dim, typename T>
vec<Dim, T> operator * (const vec<Dim, T>& lhs, T rhs) {
	vec<Dim, T> r(lhs);
	r *= rhs;
	return r;
}

template <int Dim, typename T>
vec<Dim, T> operator * (T lhs, const vec<Dim, T>& rhs) {
	return rhs * lhs;
}

template <int Dim, typename T>
bool operator == (const ymse::vec<Dim, T>& lhs, const ymse::vec<Dim, T>& rhs) {
	bool equals = true;
	for (int i=0; i<Dim; ++i) equals &= (lhs[i] == rhs[i]);
	return equals;
}

template <int Dim, typename T>
bool operator != (const ymse::vec<Dim, T>& lhs, const ymse::vec<Dim, T>& rhs) {
	return !(lhs == rhs);
}

template <int Dim, typename T>
bool operator < (const ymse::vec<Dim, T>& lhs, const ymse::vec<Dim, T>& rhs) {
	for (int i=0; i<Dim; ++i) {
		if (lhs[i] < rhs[i]) return true;
		if (rhs[i] < lhs[i]) return false;
	}
	return false;
}

#define INSTANTIATE(name, dim, t) \
	template struct vec<dim, t>; \
	template name operator + (const name&, const name&); \
	template name operator * (const name&, t); \
	template name operator * (t, const name&); \
	template name operator - (const name&, const name&); \
	template name operator / (const name&, const name&); \
	template bool operator == (const name&, const name&); \
	template bool operator != (const name&, const name&); \
	template bool operator < (const name&, const name&);

INSTANTIATE(vec2f, 2, float);
INSTANTIATE(vec3f, 3, float);
INSTANTIATE(vec4f, 4, float);
INSTANTIATE(vec2d, 2, double);
INSTANTIATE(vec2i, 2, int);

}
