#include "opposite_keys_listener.hpp"
#include "signaling_opposite_keys.hpp"

namespace ymse {

signaling_opposite_keys::signaling_opposite_keys(
	bindable_keyboard_handler& kbd,
	int neg, int pos,
	opposite_keys_listener& listener_
) :
	opposite_keys(kbd, neg, pos),
	listener(listener_)
{
}

signaling_opposite_keys::~signaling_opposite_keys() { }

void signaling_opposite_keys::set(bool* key, bool state) {
	opposite_keys::set(key, state);
	listener.set_val(val());
}

} // namespace ymse
