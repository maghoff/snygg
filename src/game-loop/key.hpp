#ifndef GAME_LOOP_KEY_HPP
#define GAME_LOOP_KEY_HPP

namespace game {

class bindable_keyboard_handler;

class key {
	bindable_keyboard_handler& kbd;
	int keysym;
	bool state;

public:
	key(bindable_keyboard_handler&, int);
	~key();

	key(const key&) = delete;
	key& operator = (const key&) = delete;

	bool val() const;
};

}

#endif
