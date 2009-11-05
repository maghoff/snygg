#include <boost/bind.hpp>
#include <ymse/bindable_keyboard_handler.hpp>
#include <ymse/keycodes.hpp>
#include <ymse/opposite_keys.hpp>
#include "player.hpp"
#include "snake.hpp"

struct player::impl {
	boost::scoped_ptr<snake> s;
	float speed;
	boost::scoped_ptr<ymse::opposite_keys> dir;
};

player::player(ymse::bindable_keyboard_handler& kbd) :
	d(new impl)
{
	d->speed = 0.5f;
	d->dir.reset(new ymse::opposite_keys(kbd, ymse::KEY_RIGHT, ymse::KEY_LEFT));
	kbd.bind(ymse::KEY_SPACE, boost::bind(&player::spawn, this, _1));
	kbd.bind(ymse::KEY_Z, boost::bind(&player::score, this, _1));
	spawn(true);
}

player::~player() {
}

void player::spawn(bool do_spawn) {
	if (do_spawn && !d->s) d->s.reset(new snake);
}

void player::score(bool do_score) {
	if (do_score) d->s->score(25-2.5*2.5);
}

void player::render(skin& sk) const {
	if (d->s) d->s->render(sk);
}

void player::move() {
	if (d->s) {
		d->s->set_turn(d->dir->val());
		d->s->forward(d->speed);
	}
}

bool player::crashes_with(intersectable_with_circle& i) const {
	return d->s && d->s->crashes_with(i);
}

void player::die() {
	d->s.reset();
}
