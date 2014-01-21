#ifndef YMSE_KEYBOARD_HANDLER_HPP
#define YMSE_KEYBOARD_HANDLER_HPP

#include <boost/noncopyable.hpp>

namespace ymse {

class keyboard_handler : public boost::noncopyable {
public:
	virtual ~keyboard_handler();

	virtual void key_event(int, bool) const = 0;
};

}

#endif
