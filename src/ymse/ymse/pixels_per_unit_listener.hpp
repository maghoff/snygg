#ifndef YMSE_PIXELS_PER_UNIT_LISTENER_HPP
#define YMSE_PIXELS_PER_UNIT_LISTENER_HPP

#include <boost/noncopyable.hpp>

namespace ymse {

class pixels_per_unit_listener : public boost::noncopyable {
public:
	virtual ~pixels_per_unit_listener();

	virtual void set_pixels_per_unit(float) = 0;
};

}

#endif
