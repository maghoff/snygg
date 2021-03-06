#include "fixed_aspect_reshaper.hpp"
#include "matrix3d.hpp"

namespace game {

fixed_aspect_reshaper::fixed_aspect_reshaper() :
	t(la::matrix3d::identity())
{
}

fixed_aspect_reshaper::~fixed_aspect_reshaper() { }

void fixed_aspect_reshaper::reshape(int width, int height) {
	t = la::matrix3d::scale(height/(double)width, 1, 1);
}

const la::matrix33f& fixed_aspect_reshaper::get_transformation() const {
	return t;
}

} // namespace game
