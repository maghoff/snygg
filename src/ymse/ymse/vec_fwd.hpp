#ifndef YMSE_VEC_FWD_HPP
#define YMSE_VEC_FWD_HPP

namespace ymse {

template <int Dim, typename T>
struct vec;

typedef vec<2, float> vec2f;
typedef vec<3, float> vec3f;
typedef vec<4, float> vec4f;

typedef vec<2, double> vec2d;

typedef vec<2, int> vec2i;

}

#endif // YMSE_VEC_FWD_HPP
