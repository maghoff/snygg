#include <SDL.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <ymse/gl.h>
#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <ymse/bindable_keyboard_handler.hpp>
#include <ymse/gl/texture.hpp>
#include <ymse/gl_box_reshaper.hpp>
#include <ymse/rect.hpp>
#include <ymse/sdl_core.hpp>
#include <ymse/keycodes.hpp>
#include <ymse/vec.hpp>
#include "gl/gl_fbo.hpp"
#include "gl/scoped_bind_fbo.hpp"
#include "metaballs.hpp"
#include "plain_skin.hpp"
#include "schematic_skin.hpp"
#include "schematic_svg_skin.hpp"
#include "textured_skin.hpp"
#include "complex_polygon.hpp"
#include "board.hpp"
#include "food_generator.hpp"
#include "item.hpp"
#include "player.hpp"
#include "paths.hpp"
#include "snygg.hpp"


#include "../config.hpp"
#ifdef HAVE_IMLIB
#include <Imlib2.h>
#endif


struct snygg::impl {
	boost::scoped_ptr<ymse::bindable_keyboard_handler> kbd;
	boost::scoped_ptr<ymse::gl_box_reshaper> reshaper;
	boost::ptr_vector<scalable_skin> skins;
	scalable_skin* active_skin;
	boost::scoped_ptr<schematic_svg_skin> svg_skin;
	boost::scoped_ptr<board> active_board;
	boost::ptr_vector<item> items;
	boost::ptr_vector<renderable> renderables;
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
	d->skins.push_back(new metaballs(&d->skins.back(), snakeskin));
	d->skins.push_back(new textured_skin(snakeskin));
	d->skins.push_back(new metaballs(&d->skins.back(), snakeskin));
	d->active_skin = &d->skins.back();

	for (size_t i=0; i<d->skins.size(); ++i) {
		d->kbd->bind_pressed(ymse::KEY_F1 + i, boost::bind(&snygg::set_skin_key, this, &d->skins[i]));
	}

	d->reshaper->set_pixels_per_unit_listener(d->active_skin);

	d->fg.reset(new food_generator(*this, *d->active_board));
	d->fg->generate();

	d->players.push_back(new player(*d->kbd, *this, *d->active_board, ymse::KEY_LEFT, ymse::KEY_RIGHT, ymse::KEY_SPACE));
	d->players.push_back(new player(*d->kbd, *this, *d->active_board, ymse::KEY_A, ymse::KEY_D, ymse::KEY_W));

	d->kbd->bind_pressed(ymse::KEY_P, boost::bind(&snygg::screenshot_key, this));
}

snygg::~snygg() {
}

void snygg::attach_to_core(ymse::sdl_core& core) {
	core.set_game_object(this);
	core.set_reshaper_object(this);
	core.set_keyboard_handler(d->kbd.get());

	d->kbd->bind_pressed(ymse::KEY_Q, boost::bind(&ymse::sdl_core::stop, &core, 0));
	d->kbd->bind_pressed(ymse::KEY_F, boost::bind(&ymse::sdl_core::toggle_fullscreen, &core));
	d->kbd->bind_pressed(ymse::KEY_H, boost::bind(&ymse::sdl_core::set_video_mode, &core, 1920, 1080, false));
	d->kbd->bind_pressed(ymse::KEY_N, boost::bind(&ymse::sdl_core::set_video_mode, &core, 1280, 720, false));
}

static void save_screenshot(const std::string& filename, unsigned char* pixels, unsigned w, unsigned h) {
#ifdef HAVE_IMLIB
	Imlib_Image img;

	img = imlib_create_image_using_data(w, h, reinterpret_cast<unsigned int*>(pixels));

	imlib_context_set_image(img);
	imlib_image_set_has_alpha(1);

	imlib_image_set_format("png");
	imlib_save_image(filename.c_str());

	imlib_free_image();
#endif
}

void snygg::take_screenshot(const std::string& filename, unsigned tex_id, unsigned w, unsigned h) {
	std::vector<unsigned char> px_buf(4*w*h);
	unsigned char* pixels = px_buf.data();

	glGetError();
	glBindTexture(GL_TEXTURE_2D, tex_id);
	assert(glGetError() == GL_NONE);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
	assert(glGetError() == GL_NONE);
	glBindTexture(GL_TEXTURE_2D, 0);
	assert(glGetError() == GL_NONE);

	for (unsigned y=0; y<h; ++y) {
		for (unsigned x=0; x<w; ++x) {
			unsigned char* px = pixels + (y*w + x) * 4;
			double a = px[3] / 255.;
			for (int i=0; i<3; ++i) px[i] /= a;
		}
	}

	for (unsigned y=0; y<h/2; ++y) {
		unsigned char* a = pixels + y*w*4;
		unsigned char* b = pixels + (h-1-y)*w*4;
		unsigned pitch = w*4;
		std::swap_ranges(a, a+pitch, b);
	}

	save_screenshot(filename, px_buf.data(), w, h);
}

void snygg::screenshot_with_skin(const std::string& filename, scalable_skin* selected_skin) {
	const unsigned screenshot_w = 1920, screenshot_h = 1080;

	const SDL_VideoInfo* vinf = SDL_GetVideoInfo();
	const unsigned w = vinf->current_w, h = vinf->current_h;
	glViewport(0, 0, screenshot_w, screenshot_h);
	reshape(screenshot_w, screenshot_h);

	gl_fbo_multisample m_fbo;

	m_fbo.set_size(screenshot_w, screenshot_h);

	scoped_bind_fbo binder(m_fbo.get_id());

	scalable_skin* prev_skin = d->active_skin;
	set_skin_key(selected_skin);
	render();
	set_skin_key(prev_skin);

	binder.unbind();

	ymse::gl::texture tx;
	glBindTexture(GL_TEXTURE_2D, tx.get_id());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenshot_w, screenshot_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	gl_fbo fbo;
	fbo.set_size(screenshot_w, screenshot_h);
	fbo.render_to(tx.get_id());
	m_fbo.blit_to(fbo);

	take_screenshot(filename, tx.get_id(), screenshot_w, screenshot_h);

	glViewport(0, 0, w, h);
	reshape(w, h);
}

static std::string format(int id, const std::string& suffix) {
	auto prefix = "screenshot-";

	std::stringstream ss;
	ss << prefix << std::setw(3) << std::setfill('0') << id << suffix;
	return ss.str();
}

void snygg::screenshot_key() {
	using namespace boost::filesystem;

	const char* suffixes[] = {
		"-schematic.png",
		"-snakeskin.png",
		"-schematic.svg"
	};

	int id = 0;
next_id:
	++id;
	for (auto& suffix : suffixes) {
		if (exists(format(id, suffix))) goto next_id;
	}

	screenshot_with_skin(format(id, suffixes[0]), &d->skins[0]);
	screenshot_with_skin(format(id, suffixes[1]), &d->skins[4]);

	// SVG screenshot:
	std::ofstream outf(format(id, suffixes[2]));
	schematic_svg_skin svg_skin(outf, d->active_board->bounding_box());

	scalable_skin* prev_skin = d->active_skin;
	set_skin_key(&svg_skin);
	render();
	set_skin_key(prev_skin);
}

void snygg::reshape(int width, int height) {
	d->reshaper->reshape(width, height);
	for (size_t i=0; i<d->skins.size(); ++i) {
		d->skins[i].load_opengl_resources(width, height);
	}
}

void snygg::set_skin_key(scalable_skin* skin) {
	d->active_skin = skin;
	d->reshaper->set_pixels_per_unit_listener(d->active_skin);
}

void snygg::render() {
	init_gl();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	d->active_skin->enter_state(skin::board_state);
	d->active_board->render(*d->active_skin);

	d->active_skin->enter_state(skin::other_state);

	for (auto& item : d->items) item.render(*d->active_skin);
	for (auto& renderable : d->renderables) renderable.render(*d->active_skin);

	d->active_skin->floor(d->active_board->floor_polygon());
}

void snygg::tick() {
	for (auto& item : d->items) {
		if (!item.is_dead()) item.move();
	}

	std::vector<player*> dead_players;
	for (auto& player : d->players) {
		for (auto& item : d->items) {
			if (!item.is_dead() && player.crashes_with(item)) item.hit_by(player);
		}
		if (player.crashes_with(*d->active_board)) {
			dead_players.push_back(&player);
		}
	}

	for (auto& dead_player : dead_players) dead_player->die();

	d->items.erase_if([](item& i) { return i.is_dead(); });
}

void snygg::add_item(std::auto_ptr<item> i) {
	d->items.push_back(i);
}

void snygg::add_renderable(std::auto_ptr<renderable> r) {
	d->renderables.push_back(r);
}
