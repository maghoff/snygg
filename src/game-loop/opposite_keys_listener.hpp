#ifndef GAME_LOOP_OPPOSITE_KEYS_LISTENER_HPP
#define GAME_LOOP_OPPOSITE_KEYS_LISTENER_HPP

namespace game {

class opposite_keys_listener {
public:
	opposite_keys_listener();
	virtual ~opposite_keys_listener();

	virtual void set_val(int) = 0;
};

} // namespace game

#endif // GAME_LOOP_OPPOSITE_KEYS_LISTENER_HPP
