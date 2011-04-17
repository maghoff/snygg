#include <SDL.h>
#include <cmath>
#include <vector>
#include <ymse/gl.h>
#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <ymse/bindable_keyboard_handler.hpp>
#include <ymse/gl_box_reshaper.hpp>
#include <ymse/rect.hpp>
#include <ymse/sdl_core.hpp>
#include <ymse/keycodes.hpp>
#include <ymse/vec.hpp>
#include "board.hpp"
#include "complex_polygon.hpp"
#include "food_generator.hpp"
#include "item.hpp"
#include "metaballs.hpp"
#include "paths.hpp"
#include "plain_skin.hpp"
#include "player.hpp"
#include "schematic_skin.hpp"
#include "snygg.hpp"
#include "textured_skin.hpp"


struct snygg::impl {
	boost::scoped_ptr<ymse::bindable_keyboard_handler> kbd;
	boost::scoped_ptr<ymse::gl_box_reshaper> reshaper;
	boost::ptr_vector<scalable_skin> skins;
	scalable_skin* active_skin;
	boost::scoped_ptr<board> active_board;
	boost::ptr_list<item> items;
	boost::ptr_list<renderable> renderables;
	boost::ptr_vector<player> players;
	boost::scoped_ptr<food_generator> fg;

	ymse::rectf metaballs_rect;
};

void init_gl() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);

	glDisable(GL_DEPTH_TEST);

	glShadeModel(GL_FLAT);

	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);

	// Modern consumer grade cards often don't implement this. Use FSAA instead. (Or in addition)
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
}

snygg::snygg(const std::string& board_filename) :
	d(new impl)
{
	using boost::filesystem::path;

	d->kbd.reset(new ymse::bindable_keyboard_handler);

	init_gl();

	path board_path(paths::find_absolute_or_in_path(board_filename, paths::levels()));
	d->active_board.reset(new board(board_path));

	d->reshaper.reset(new ymse::gl_box_reshaper);
	ymse::rectf bb = d->active_board->bounding_box();
	const float margin = 5.f;
	d->reshaper->set_box(bb.x1 - margin, bb.y1 - margin, bb.x2 + margin, bb.y2 + margin);

	d->metaballs_rect.x1 = bb.x1 - margin;
	d->metaballs_rect.y1 = bb.y1 - margin;
	d->metaballs_rect.x2 = bb.x2 + margin;
	d->metaballs_rect.y2 = bb.y2 + margin;

	std::string snakeskin = (paths::skins() / "snakeskin").string();
	d->skins.push_back(new schematic_skin);
	d->skins.push_back(new plain_skin);
	d->skins.push_back(new metaballs<plain_skin>(snakeskin));
	d->skins.push_back(new textured_skin(snakeskin));
	d->skins.push_back(new metaballs<textured_skin>(snakeskin));
	d->active_skin = &d->skins.back();

	for (size_t i=0; i<d->skins.size(); ++i) {
		d->kbd->bind_pressed(ymse::KEY_F1 + i, boost::bind(&snygg::set_skin_key, this, &d->skins[i]));
	}

	d->reshaper->set_pixels_per_unit_listener(d->active_skin);

	d->fg.reset(new food_generator(*this, *d->active_board));
	d->fg->generate();

	d->players.push_back(new player(*d->kbd, *this, *d->active_board));
}

snygg::~snygg() {
}

void snygg::attach_to_core(ymse::sdl_core& core) {
	core.set_game_object(this);
	core.set_reshaper_object(this);
	core.set_keyboard_handler(d->kbd.get());

	d->kbd->bind_pressed(ymse::KEY_Q, boost::bind(&ymse::sdl_core::stop, &core, 0));
	d->kbd->bind_pressed(ymse::KEY_F, boost::bind(&ymse::sdl_core::toggle_fullscreen, &core));
}

void snygg::reshape(int width, int height) {
	d->reshaper->reshape(width, height);
	for (size_t i=0; i<d->skins.size(); ++i) {
		d->skins[i].load_opengl_resources();
	}
}

void snygg::set_skin_key(scalable_skin* skin) {
	d->active_skin = skin;
	d->reshaper->set_pixels_per_unit_listener(d->active_skin);
}

void snygg::render() {
	typedef boost::ptr_vector<player>::iterator piter;
	typedef boost::ptr_list<item>::iterator iiter;
	typedef boost::ptr_list<renderable>::iterator riter;

	init_gl();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	d->active_board->render(*d->active_skin);

	iiter iend = d->items.end();
	for (iiter i = d->items.begin(); i != iend; ++i) {
		i->render(*d->active_skin);
	}

	riter rend = d->renderables.end();
	for (riter i = d->renderables.begin(); i != rend; ++i) {
		i->render(*d->active_skin);
	}

	d->active_skin->floor(d->active_board->floor_polygon());
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
		(*i)->die();
	}

	for (iiter i = d->items.begin(), j; i != iend; i = j) {
		j = i;
		++j;
		if (i->is_dead()) d->items.erase(i);
	}
}

void snygg::add_item(std::auto_ptr<item> i) {
	d->items.push_back(i);
}

void snygg::add_renderable(std::auto_ptr<renderable> r) {
	d->renderables.push_back(r);
}
