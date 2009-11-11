#include <boost/bind.hpp>
#include <ymse/bindable_keyboard_handler.hpp>
#include <ymse/keycodes.hpp>
#include <ymse/opposite_keys.hpp>
#include "item_container.hpp"
#include "dead_player.hpp"
#include "player.hpp"
#include "snake.hpp"

struct player::impl {
	item_container& ic;
	std::auto_ptr<snake> s;
	float speed;
	boost::scoped_ptr<ymse::opposite_keys> dir;

	impl(item_container& ic_) : ic(ic_) { }
};

player::player(ymse::bindable_keyboard_handler& kbd, item_container& ic) :
	d(new impl(ic))
{
	d->speed = 0.5f;
	d->dir.reset(new ymse::opposite_keys(kbd, ymse::KEY_RIGHT, ymse::KEY_LEFT));
	kbd.bind(ymse::KEY_SPACE, boost::bind(&player::spawn, this, _1));
	spawn(true);
}

player::~player() {
}

void player::spawn(bool do_spawn) {
	if (do_spawn && !d->s.get()) d->s.reset(new snake);
}

void player::render(skin& sk) const {
	if (d->s.get()) d->s->render(sk);
}

void player::move() {
	if (d->s.get()) {
		d->s->set_turn(d->dir->val());
		d->s->forward(d->speed);
	}
}

bool player::crashes_with(intersectable_with_circle& i) const {
	return d->s.get() && d->s->crashes_with(i);
}

void player::die() {
	d->ic.add_item(std::auto_ptr<item>(new dead_player(d->s, d->ic)));
}

void player::score() {
	d->s->score(25-2.5*2.5);
}

