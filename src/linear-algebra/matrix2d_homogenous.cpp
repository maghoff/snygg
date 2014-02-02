#include "matrix3d.hpp"
#include "matrix2d_homogenous.hpp"

namespace la {
namespace matrix2d {
namespace homogenous {

matrix33f scale(double s) {
	return scale(s, s);
}

matrix33f scale(double sx, double sy) {
	return matrix3d::scale(sx, sy, 1);
}

matrix33f translate(double dx, double dy) {
	matrix33f t = matrix3d::identity();
	t[0][2] = dx;
	t[1][2] = dy;
	return t;
}

matrix33f rotate(double ang) {
	return matrix3d::rotate_z(ang);
}

matrix44f as_3d_homogenous(const la::matrix33f& m) {
	la::matrix44f res;

	res[0][0] = m[0][0];
	res[0][1] = m[0][1];
	res[0][2] = 0;
	res[0][3] = m[0][2];

	res[1][0] = m[1][0];
	res[1][1] = m[1][1];
	res[1][2] = 0;
	res[1][3] = m[1][2];

	res[2][0] = 0;
	res[2][1] = 0;
	res[2][2] = 1;
	res[2][3] = 0;

	res[3][0] = m[2][0];
	res[3][1] = m[2][1];
	res[3][2] = 0;
	res[3][3] = m[2][2];

	return res;
}

} // namespace homogenous
} // namespace matrix2d
} // namespace la
