#include "fixed_aspect_reshaper.hpp"
#include "matrix3d.hpp"

namespace ymse {

fixed_aspect_reshaper::fixed_aspect_reshaper() :
	t(ymse::matrix3d::identity())
{
}

fixed_aspect_reshaper::~fixed_aspect_reshaper() { }

void fixed_aspect_reshaper::reshape(int width, int height) {
	t = ymse::matrix3d::scale(height/(double)width, 1, 1);
}

const matrix33f& fixed_aspect_reshaper::get_transformation() const {
	return t;
}

} // namespace ymse
