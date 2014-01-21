#include "matrix.hpp"

namespace ymse {

template <int Size, typename T>
sq_matrix<Size, T>::sq_matrix() {
}

template <int Size, typename T>
sq_matrix<Size, T>::sq_matrix(T initial) {
	for (int i=0; i<Size*Size; ++i) v[i] = initial;
}

template <int Size, typename T>
sq_matrix<Size, T> operator * (const sq_matrix<Size, T>& lhs, const sq_matrix<Size, T>& rhs) {
	sq_matrix<Size, T> res;
	for (int row = 0; row < Size; ++row) {
		for (int col = 0; col < Size; ++col) {
			T& d = res[row][col];
			d = 0;
			for (int i = 0; i < Size; ++i) {
				d += lhs[row][i] * rhs[i][col];
			}
		}
	}
	return res;
}

template <int Size, typename T>
vec<Size, T> operator * (const sq_matrix<Size, T>& lhs, const vec<Size, T>& rhs) {
	vec<Size, T> r;
	for (int row = 0; row < Size; ++row) {
		T& d = r[row];
		d = 0;
		for (int i = 0; i < Size; ++i) {
			d += lhs[row][i] * rhs[i];
		}
	}
	return r;
}


template struct sq_matrix<3, float>;
template sq_matrix<3, float> operator * (const sq_matrix<3, float>&, const sq_matrix<3, float>&);
template vec<3, float> operator * (const sq_matrix<3, float>&, const vec<3, float>&);

template struct sq_matrix<2, float>;
template sq_matrix<2, float> operator * (const sq_matrix<2, float>&, const sq_matrix<2, float>&);
template vec<2, float> operator * (const sq_matrix<2, float>&, const vec<2, float>&);

}
