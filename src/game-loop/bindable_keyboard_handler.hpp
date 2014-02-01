#ifndef YMSE_BINDABLE_KEYBOARD_HANDLER_HPP
#define YMSE_BINDABLE_KEYBOARD_HANDLER_HPP

#include <map>
#include <functional>
#include "keyboard_handler.hpp"

namespace game {

class bindable_keyboard_handler : public keyboard_handler {
	typedef std::function<void(bool)> callback;
	typedef std::function<void()> simple_callback;

private:
	std::map<int, callback> m;

public:
	bindable_keyboard_handler();
	~bindable_keyboard_handler();

	void key_event(int, bool) const;

	void bind(int, callback);
	void bind_pressed(int, simple_callback);
	void unbind(int);
};

}

#endif
