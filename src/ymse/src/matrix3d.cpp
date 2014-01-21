#include <cmath>
#include "matrix.hpp"
#include "matrix3d.hpp"
#include "vec.hpp"

namespace ymse {
namespace matrix3d {

matrix33f identity() {
	return scale(1);
}

matrix33f scale(double x) {
	matrix33f m(0);
	for (int i=0; i<3; ++i) m[i][i] = x;
	return m;
}

matrix33f scale(double x, double y, double z) {
	matrix33f m(0);
	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
	return m;
}

matrix33f rotate_x(double th) {
	matrix33f m(identity());
	m[1][1] =  cos(th); m[1][2] =  sin(th);
	m[2][1] = -sin(th); m[2][2] =  cos(th);
	return m;
}

matrix33f rotate_y(double th) {
	matrix33f m(identity());
	m[2][2] =  cos(th); m[2][0] =  sin(th);
	m[0][2] = -sin(th); m[0][0] =  cos(th);
	return m;
}

matrix33f rotate_z(double th) {
	matrix33f m(identity());
	m[0][0] =  cos(th); m[0][1] =  sin(th);
	m[1][0] = -sin(th); m[1][1] =  cos(th);
	return m;
}

vec3f operator * (const matrix33f& lhs, const vec3f& rhs) {
	vec3f r;
	for (int row = 0; row < 3; ++row) {
		float& d = r[row];
		d = 0;
		for (int i = 0; i < 3; ++i) {
			d += lhs[row][i] * rhs[i];
		}
	}
	return r;
}

}}
