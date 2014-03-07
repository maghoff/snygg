#ifndef SCALABLE_SKIN_HPP
#define SCALABLE_SKIN_HPP

#include <pixels_per_unit_listener.hpp>
#include <matrix.hpp>
#include "skin.hpp"

class scalable_skin : public skin, public game::pixels_per_unit_listener {
protected:
	float detail_modifier;

	float get_step_size(float r);

public:
	scalable_skin();

	virtual void set_transformation(const la::matrix33f&) = 0;

	void set_pixels_per_unit(float);
};

#endif // SCALABLE_SKIN_HPP
