#ifndef YMSE_SIGNALING_OPPOSITE_KEYS_HPP
#define YMSE_SIGNALING_OPPOSITE_KEYS_HPP

#include "opposite_keys.hpp"

namespace ymse {

class opposite_keys_listener;

class signaling_opposite_keys : public ymse::opposite_keys {
	opposite_keys_listener& listener;

protected:
	void set(bool*, bool);

public:
	signaling_opposite_keys(bindable_keyboard_handler&, int neg, int pos, opposite_keys_listener&);
	~signaling_opposite_keys();
};

} // namespace ymse

#endif // YMSE_SIGNALING_OPPOSITE_KEYS_HPP
