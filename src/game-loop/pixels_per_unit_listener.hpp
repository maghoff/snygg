#ifndef YMSE_PIXELS_PER_UNIT_LISTENER_HPP
#define YMSE_PIXELS_PER_UNIT_LISTENER_HPP

namespace game {

class pixels_per_unit_listener {
public:
	pixels_per_unit_listener() = default;
	virtual ~pixels_per_unit_listener();

	pixels_per_unit_listener(const pixels_per_unit_listener&) = delete;
	pixels_per_unit_listener& operator = (const pixels_per_unit_listener&) = delete;

	virtual void set_pixels_per_unit(float) = 0;
};

}

#endif
