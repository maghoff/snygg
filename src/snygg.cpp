#include <cmath>
#include <vector>
#include <GL/gl.h>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <ymse/bindable_keyboard_handler.hpp>
#include "board.hpp"
#include "dead_player.hpp"
#include "food_generator.hpp"
#include "item.hpp"
#include "plain_skin.hpp"
#include "player.hpp"
#include "snygg.hpp"


struct snygg::impl {
	boost::scoped_ptr<skin> active_skin;
	boost::scoped_ptr<board> active_board;
	boost::ptr_list<item> items;
	boost::ptr_vector<player> players;
	boost::scoped_ptr<ymse::bindable_keyboard_handler> kbd;
	boost::scoped_ptr<food_generator> fg;
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

	d->fg.reset(new food_generator(*this, *d->active_board));
	for (int i=0; i<10; ++i) d->fg->generate();

	d->players.push_back(new player(*d->kbd));
}

snygg::~snygg() {
}


void snygg::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	d->active_board->render(*d->active_skin);

	typedef boost::ptr_vector<player>::iterator piter;
	typedef boost::ptr_list<item>::iterator iiter;

	iiter iend = d->items.end();
	for (iiter i = d->items.begin(); i != iend; ++i) {
		i->render(*d->active_skin);
	}

	piter pend = d->players.end();
	for (piter i = d->players.begin(); i != pend; ++i) {
		i->render(*d->active_skin);
	}
}

void snygg::tick() {
	typedef boost::ptr_vector<player>::iterator piter;
	typedef boost::ptr_list<item>::iterator iiter;
	typedef std::vector<player*> dead_players_c;
	typedef std::vector<player*>::iterator iter_d;

	const iiter iend = d->items.end();
	const piter pend = d->players.end();

	for (iiter i = d->items.begin(); i != iend; ++i) {
		if (!i->is_dead()) i->move();
	}

	for (piter i = d->players.begin(); i != pend; ++i) {
		i->move();
	}

	dead_players_c dead_players;
	for (piter i = d->players.begin(); i != pend; ++i) {
		for (iiter j = d->items.begin(); j != iend; ++j) {
			if (!j->is_dead() && i->crashes_with(*j)) j->hit_by(*i);
		}
		if (i->crashes_with(*d->active_board)) {
			dead_players.push_back(&*i);
		}
	}

	iter_d end_d = dead_players.end();
	for (iter_d i = dead_players.begin(); i != end_d; ++i) {
		d->items.push_back((*i)->die());
	}

	for (iiter i = d->items.begin(), j; i != iend; i = j) {
		j = i;
		++j;
		if (i->is_dead()) d->items.erase(i);
	}
}

ymse::keyboard_handler* snygg::get_keyboard_handler() {
	return &*d->kbd;
}

void snygg::add_item(std::auto_ptr<item> i) {
	d->items.push_back(i);
}
