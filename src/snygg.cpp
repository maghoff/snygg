#include <cmath>
#include <GL/gl.h>
#include <ymse/bindable_keyboard_handler.hpp>
#include <ymse/opposite_keys.hpp>
#include <ymse/keycodes.hpp>
#include "board.hpp"
#include "plain_skin.hpp"
#include "snake.hpp"
#include "snygg.hpp"


struct snygg::impl {
	boost::scoped_ptr<skin> active_skin;
	boost::scoped_ptr<board> active_board;
	boost::scoped_ptr<snake> player;

	boost::scoped_ptr<ymse::bindable_keyboard_handler> kbd;
	boost::scoped_ptr<ymse::opposite_keys> dir;
};


snygg::snygg() :
	d(new impl)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);

	glDisable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	d->active_skin.reset(new plain_skin);

	d->active_board.reset(new board);

	d->player.reset(new snake);

	d->kbd.reset(new ymse::bindable_keyboard_handler);

	d->dir.reset(new ymse::opposite_keys(*d->kbd, ymse::KEY_RIGHT, ymse::KEY_LEFT));
}

snygg::~snygg() {
}


void snygg::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	d->active_board->render(*d->active_skin);

	d->player->render(*d->active_skin);
}

void snygg::tick() {
	d->player->set_turn(d->dir->val());
	d->player->forward(0.5);
}

ymse::keyboard_handler* snygg::get_keyboard_handler() {
	return &*d->kbd;
}

