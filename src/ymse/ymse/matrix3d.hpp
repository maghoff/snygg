#ifndef YMSE_MATRIX3D_HPP
#define YMSE_MATRIX3D_HPP

namespace ymse {

template <int Size, typename T>
struct sq_matrix;

typedef sq_matrix<3, float> matrix33f;

template <int dim, typename T>
struct vec;

typedef vec<3, float> vec3f;


namespace matrix3d {

matrix33f identity();

matrix33f scale(double);
matrix33f scale(double x, double y, double z);

matrix33f rotate_x(double);
matrix33f rotate_y(double);
matrix33f rotate_z(double);

vec3f operator * (const matrix33f& lhs, const vec3f& rhs);

}

}

#endif
