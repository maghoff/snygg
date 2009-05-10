#include <cmath>
#include <GL/gl.h>
#include <ymse/bindable_keyboard_handler.hpp>
#include <ymse/opposite_keys.hpp>
#include <ymse/keycodes.hpp>
#include "board.hpp"
#include "plain_skin.hpp"
#include "snake.hpp"
#include "snygg.hpp"

snygg::snygg() {
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

	active_skin.reset(new plain_skin);

	active_board.reset(new board);

	player.reset(new snake);

	kbd.reset(new ymse::bindable_keyboard_handler);

	dir.reset(new ymse::opposite_keys(*kbd, ymse::KEY_RIGHT, ymse::KEY_LEFT));
}

snygg::~snygg() {
}


void snygg::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	active_board->render(*active_skin);

	player->render(*active_skin);
}

void snygg::tick() {
	player->set_turn(dir->val());
	player->forward(0.5);
}

ymse::keyboard_handler* snygg::get_keyboard_handler() {
	return &*kbd;
}

