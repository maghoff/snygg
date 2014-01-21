#ifndef FIXED_ASPECT_RESHAPER_HPP
#define FIXED_ASPECT_RESHAPER_HPP

#include "reshaper.hpp"
#include "matrix.hpp"

namespace ymse {

class fixed_aspect_reshaper : public ymse::reshaper {
	matrix33f t;

public:
	fixed_aspect_reshaper();
	~fixed_aspect_reshaper();

	void reshape(int width, int height);
	const matrix33f& get_transformation() const;
};

} // namespace ymse

#endif // FIXED_ASPECT_RESHAPER_HPP
