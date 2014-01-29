#include <boost/bind.hpp>
#include <ymse/bindable_keyboard_handler.hpp>
#include <ymse/keycodes.hpp>
#include <ymse/opposite_keys_listener.hpp>
#include <ymse/signaling_opposite_keys.hpp>
#include "board.hpp"
#include "item_container.hpp"
#include "snake.hpp"
#include "player.hpp"
#include "snake_direction_listener.hpp"

struct player::impl {
	item_container& ic;
	snake* s;
	std::unique_ptr<snake_direction_listener> del;
	std::unique_ptr<ymse::signaling_opposite_keys> dir;
	float speed;
	board& game_board;

	impl(item_container& ic_, board& _board) : ic(ic_), game_board(_board) { }
};

player::player(
	ymse::bindable_keyboard_handler& kbd,
	item_container& ic, board& _board,
	int left, int right, int spawn
) :
	d(new impl(ic, _board))
{
	d->s = 0;
	d->speed = 0.4f;
	d->del.reset(new snake_direction_listener);
	d->dir.reset(new ymse::signaling_opposite_keys(kbd, right, left, *d->del));
	kbd.bind(spawn, boost::bind(&player::spawn_key, this, _1));
}

player::~player() {
}

void player::spawn_key(bool do_spawn) {
	if (do_spawn) spawn();
}

void player::spawn() {
	if (!d->s) {
		d->ic.add_item(std::unique_ptr<item>(d->s = new snake(d->ic, d->speed, d->game_board.get_starting_position())));
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
	d->s->crack_head();
	d->s = 0;
}

void player::score() {
	d->s->score(5*5 - 2.5*2.5);
}
