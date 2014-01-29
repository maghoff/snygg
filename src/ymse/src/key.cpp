#include "bindable_keyboard_handler.hpp"
#include "key.hpp"

namespace ymse {

key::key(bindable_keyboard_handler& kbd_, int keysym_) :
	kbd(kbd_),
	keysym(keysym_),
	state(false)
{
	kbd.bind(keysym, [=](bool pressed){ state = pressed; });
}


key::~key() {
	kbd.unbind(keysym);
}

bool key::val() const {
	return state;
}

}
