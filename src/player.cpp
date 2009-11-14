#include <boost/bind.hpp>
#include <ymse/bindable_keyboard_handler.hpp>
#include <ymse/keycodes.hpp>
#include <ymse/opposite_keys_listener.hpp>
#include <ymse/signaling_opposite_keys.hpp>
#include "item_container.hpp"
#include "dead_player.hpp"
#include "player.hpp"
#include "snake.hpp"
#include "snake_direction_listener.hpp"

struct player::impl {
	item_container& ic;
	snake* s;
	boost::scoped_ptr<snake_direction_listener> del;
	boost::scoped_ptr<ymse::signaling_opposite_keys> dir;
	float speed;

	impl(item_container& ic_) : ic(ic_) { }
};

player::player(ymse::bindable_keyboard_handler& kbd, item_container& ic) :
	d(new impl(ic))
{
	d->s = 0;
	d->speed = 0.5f;
	d->del.reset(new snake_direction_listener);
	d->dir.reset(new ymse::signaling_opposite_keys(kbd, ymse::KEY_RIGHT, ymse::KEY_LEFT, *d->del));
	kbd.bind(ymse::KEY_SPACE, boost::bind(&player::spawn, this, _1));
	spawn(true);
}

player::~player() {
}

void player::spawn(bool do_spawn) {
	if (do_spawn && !d->s) {
		d->ic.add_item(item_ptr(d->s = new snake(d->speed)));
		d->s->set_turn(d->dir->val());
		d->del->set_target(d->s);
	}
}

bool player::crashes_with(intersectable_with_circle& i) const {
	if (d->s) return d->s->crashes_with(i);
	else return false;
}

void player::die() {
	d->del->set_target(0);
	d->ic.add_item(std::auto_ptr<item>(new dead_player(d->s, d->ic)));
	d->s = 0;
}

void player::score() {
	d->s->score(5*5 - 2.5*2.5);
}
