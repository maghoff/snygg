#ifndef FIXED_ASPECT_RESHAPER_HPP
#define FIXED_ASPECT_RESHAPER_HPP

#include "reshaper.hpp"
#include "matrix.hpp"

namespace game {

class fixed_aspect_reshaper : public game::reshaper {
	la::matrix33f t;

public:
	fixed_aspect_reshaper();
	~fixed_aspect_reshaper();

	void reshape(int width, int height);
	const la::matrix33f& get_transformation() const;
};

} // namespace game

#endif // FIXED_ASPECT_RESHAPER_HPP
