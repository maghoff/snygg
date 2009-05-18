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
	d->s.reset(new snake);
}

player::~player() {
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
