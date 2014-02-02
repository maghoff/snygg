#ifndef GAME_LOOP_MOUSE_HANDLER_HPP
#define GAME_LOOP_MOUSE_HANDLER_HPP

namespace game {

class mouse_handler {
public:
	virtual ~mouse_handler();

	mouse_handler(const mouse_handler&) = delete;
	mouse_handler& operator = (const mouse_handler&) = delete;

	virtual void mouse_motion(int dx, int dy, int x, int y) = 0;
	virtual void mouse_button_down(int button, int x, int y) = 0;
	virtual void mouse_button_up(int button, int x, int y) = 0;
};

}

#endif
