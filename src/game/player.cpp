#include "player.hpp"
#include <bindable_keyboard_handler.hpp>
#include <keycodes.hpp>
#include <opposite_keys_listener.hpp>
#include <signaling_opposite_keys.hpp>
#include "board.hpp"
#include "item_container.hpp"
#include "snake.hpp"
#include "snake_direction_listener.hpp"
#include "score_listener.hpp"

struct player::impl {
	item_container& ic;
	snake* s;
	std::unique_ptr<snake_direction_listener> del;
	std::unique_ptr<game::signaling_opposite_keys> dir;
	float speed;
	board& game_board;

	score_listener& score_reporter;
	int score;

	impl(
		item_container& ic_,
		board& board_,
		score_listener& score_reporter_
	) :
		ic(ic_),
		game_board(board_),
		score_reporter(score_reporter_)
	{ }
};

player::player(
	game::bindable_keyboard_handler& kbd,
	item_container& ic, board& _board,
	score_listener& score_reporter,
	int left, int right, int spawnkey
) :
	d(new impl(ic, _board, score_reporter))
{
	d->s = 0;
	d->speed = 0.4f;
	d->del.reset(new snake_direction_listener);
	d->dir.reset(new game::signaling_opposite_keys(kbd, right, left, *d->del));
	kbd.bind_pressed(spawnkey, [=]{ spawn(); });
}

player::~player() {
}

void player::spawn() {
	if (!d->s) {
		d->score = 0;
		d->ic.add_item(std::unique_ptr<item>(d->s = new snake(d->ic, d->speed, d->game_board.get_starting_position())));
		d->s->set_turn(d->dir->val());
		d->del->set_target(d->s);

		d->score_reporter.score_updated(d->score);
	}
}

bool player::crashes_with(const intersectable_with_circle& i) const {
	if (d->s) return d->s->crashes_with(i);
	else return false;
}

void player::die() {
	d->del->set_target(0);
	d->s->crack_head();
	d->s = 0;

	d->score_reporter.died(d->score);
}

void player::score() {
	d->s->score(5*5 - 2.5*2.5);
	++d->score;
	d->score_reporter.score_updated(d->score);
}
