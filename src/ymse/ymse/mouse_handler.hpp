#ifndef YMSE_MOUSE_HANDLER_HPP
#define YMSE_MOUSE_HANDLER_HPP

#include <boost/noncopyable.hpp>

namespace ymse {

class mouse_handler : public boost::noncopyable {
public:
	virtual ~mouse_handler();

	virtual void mouse_motion(int dx, int dy, int x, int y) = 0;
	virtual void mouse_button_down(int button, int x, int y) = 0;
	virtual void mouse_button_up(int button, int x, int y) = 0;
};

}

#endif
