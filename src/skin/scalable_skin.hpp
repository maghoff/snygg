#ifndef SCALABLE_SKIN_HPP
#define SCALABLE_SKIN_HPP

#include <ymse/pixels_per_unit_listener.hpp>
#include "skin.hpp"

class scalable_skin : public skin, public ymse::pixels_per_unit_listener {
protected:
	float detail_modifier;

	float get_step_size(float r);

public:
	scalable_skin();

	void set_pixels_per_unit(float);
};

#endif // SCALABLE_SKIN_HPP
