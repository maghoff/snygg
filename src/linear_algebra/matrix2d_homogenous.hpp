#ifndef MATRIX2D_HOMOGENOUS_HPP
#define MATRIX2D_HOMOGENOUS_HPP

#include "matrix.hpp"

namespace la {
namespace matrix2d {
namespace homogenous {

matrix33f scale(double s);
matrix33f scale(double sx, double sy);
matrix33f translate(double dx, double dy);
matrix33f rotate(double ang);

} // namespace homogenous
} // namespace matrix2d
} // namespace la

#endif // MATRIX2D_HOMOGENOUS_HPP
