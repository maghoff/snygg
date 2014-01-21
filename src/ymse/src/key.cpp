#include <boost/bind.hpp>
#include "bindable_keyboard_handler.hpp"
#include "key.hpp"

namespace ymse {

namespace {

void set(bool* s, bool pos) { *s = pos; }

}

key::key(bindable_keyboard_handler& kbd_, int keysym_) :
	kbd(kbd_),
	keysym(keysym_),
	state(false)
{
	kbd.bind(keysym, boost::bind(&set, &state, _1));
}


key::~key() {
	kbd.unbind(keysym);
}

bool key::val() const {
	return state;
}

}
