#ifndef YMSE_KEYBOARD_HANDLER_HPP
#define YMSE_KEYBOARD_HANDLER_HPP

namespace game {

class keyboard_handler {
public:
	keyboard_handler() = default;
	virtual ~keyboard_handler();

	keyboard_handler(const keyboard_handler&) = delete;
	keyboard_handler& operator = (const keyboard_handler&) = delete;

	virtual void key_event(int, bool) const = 0;
};

}

#endif
