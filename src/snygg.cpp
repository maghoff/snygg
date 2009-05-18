#include <cmath>
#include <vector>
#include <GL/gl.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include <ymse/bindable_keyboard_handler.hpp>
#include "board.hpp"
#include "plain_skin.hpp"
#include "player.hpp"
#include "snygg.hpp"


struct snygg::impl {
	boost::scoped_ptr<skin> active_skin;
	boost::scoped_ptr<board> active_board;
	boost::ptr_vector<player> players;
	boost::scoped_ptr<ymse::bindable_keyboard_handler> kbd;
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

	d->kbd.reset(new ymse::bindable_keyboard_handler);

	d->players.push_back(new player(*d->kbd));
}

snygg::~snygg() {
}


void snygg::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	d->active_board->render(*d->active_skin);

	typedef boost::ptr_vector<player>::const_iterator iter;
	iter end = d->players.end();
	for (iter i = d->players.begin(); i != end; ++i) {
		i->render(*d->active_skin);
	}
}

void snygg::tick() {
	typedef boost::ptr_vector<player>::iterator iter;
	iter end = d->players.end();
	for (iter i = d->players.begin(); i != end; ++i) {
		i->move();
	}

	typedef std::vector<player*> dead_players_c;
	dead_players_c dead_players;
	for (iter i = d->players.begin(); i != end; ++i) {
		if (i->crashes_with(*d->active_board)) {
			dead_players.push_back(&*i);
		}
	}

	typedef std::vector<player*>::iterator iter_d;
	iter_d end_d = dead_players.end();
	for (iter_d i = dead_players.begin(); i != end_d; ++i) {
		(*i)->die();
	}
}

ymse::keyboard_handler* snygg::get_keyboard_handler() {
	return &*d->kbd;
}
