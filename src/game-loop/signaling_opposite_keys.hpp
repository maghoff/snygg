#ifndef GAME_LOOP_SIGNALING_OPPOSITE_KEYS_HPP
#define GAME_LOOP_SIGNALING_OPPOSITE_KEYS_HPP

#include "opposite_keys.hpp"

namespace game {

class opposite_keys_listener;

class signaling_opposite_keys : public game::opposite_keys {
	opposite_keys_listener& listener;

protected:
	void set(bool*, bool);

public:
	signaling_opposite_keys(bindable_keyboard_handler&, int neg, int pos, opposite_keys_listener&);
	~signaling_opposite_keys();
};

} // namespace game

#endif // GAME_LOOP_SIGNALING_OPPOSITE_KEYS_HPP
