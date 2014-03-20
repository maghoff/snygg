#include <SDL.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <GL/glew.h>

#include <boost/filesystem/operations.hpp>
#include <keycodes.hpp>
#include <bindable_keyboard_handler.hpp>
#include <texture.hpp>
#include <box_reshaper.hpp>
#include <rect.hpp>
#include <sdl_core.hpp>
#include <vec.hpp>
#include <matrix2d_homogenous.hpp>
#include "gl/gl_fbo.hpp"
#include "gl/scoped_bind_fbo.hpp"
#include "metaballs.hpp"
#include "plain_skin.hpp"
#include "schematic_skin.hpp"
#include "schematic_svg_skin.hpp"
#include "textured_skin.hpp"
#include "complex_polygon.hpp"
#include "board.hpp"
#include "lua_board_provider.hpp"
#include "filesystem_file_loader.hpp"
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
	std::unique_ptr<game::bindable_keyboard_handler> kbd;

	std::unique_ptr<game::box_reshaper> reshaper;
	std::vector<std::unique_ptr<scalable_skin>> skins;
	scalable_skin* active_skin;
	std::unique_ptr<schematic_svg_skin> svg_skin;

	std::unique_ptr<board_provider> active_board_provider;
	std::unique_ptr<board> active_board;
	std::vector<std::unique_ptr<item>> items;
	std::vector<std::unique_ptr<item>> new_items;
	std::vector<std::unique_ptr<renderable>> renderables;
	std::vector<std::unique_ptr<player>> players;

	std::unique_ptr<food_generator> fg;
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

	glEnable(GL_MULTISAMPLE);
}

snygg::snygg(const std::string& board_filename) :
	d(new impl)
{
	using boost::filesystem::path;

	d->kbd.reset(new game::bindable_keyboard_handler);

	init_gl();

	path board_path(paths::find_absolute_or_in_path(board_filename, paths::levels()));
	filesystem_file_loader loader;
	loader.add_search_path(board_path.branch_path().string());
	loader.add_search_path(paths::levels().string());
	d->active_board_provider.reset(new lua_board_provider(loader, board_path.string()));
	d->active_board.reset(new board(*d->active_board_provider));

	d->reshaper.reset(new game::box_reshaper);
	rectf bb = d->active_board->bounding_box();
	const float margin = 5.f;
	d->reshaper->set_box(bb.x1 - margin, bb.y1 - margin, bb.x2 + margin, bb.y2 + margin);

	std::string snakeskin = (paths::skins() / "snakeskin").string();
	d->skins.emplace_back(new schematic_skin);
	d->skins.emplace_back(new plain_skin);
	d->skins.emplace_back(new metaballs(d->skins.back().get(), snakeskin));
	d->skins.emplace_back(new textured_skin(snakeskin));
	d->skins.emplace_back(new metaballs(d->skins.back().get(), snakeskin));
	d->active_skin = d->skins.back().get();

	for (size_t i=0; i<d->skins.size(); ++i) {
		d->kbd->bind_pressed(game::KEY_F1 + i, [=]{ set_skin_key(d->skins[i].get()); });
	}

	d->fg.reset(new food_generator(*this, *d->active_board));
	d->fg->generate();

	d->players.emplace_back(new player(*d->kbd, *this, *d->active_board, game::KEY_LEFT, game::KEY_RIGHT, game::KEY_SPACE));
	d->players.emplace_back(new player(*d->kbd, *this, *d->active_board, game::KEY_A, game::KEY_D, game::KEY_W));

	d->kbd->bind_pressed(game::KEY_P, [=]{ screenshot_key(); });
}

snygg::~snygg() {
}

void snygg::attach_to_core(game::sdl_core& core) {
	core.set_tick_handler(this);
	core.set_reshaper_object(this);
	core.set_keyboard_handler(d->kbd.get());

	// Is capturing a reference by reference actually safe?
	// See http://stackoverflow.com/questions/21443023/capturing-a-reference-by-reference-in-a-c11-lambda
	const auto c = &core;
	d->kbd->bind_pressed(game::KEY_Q, [=]{ c->stop(0); });
	d->kbd->bind_pressed(game::KEY_F, [=]{ c->toggle_fullscreen(); });
	d->kbd->bind_pressed(game::KEY_H, [=]{ c->set_video_mode(1920, 1080, false); });
	d->kbd->bind_pressed(game::KEY_N, [=]{ c->set_video_mode(1280, 720, false); });
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

	gl::texture tx;
	glBindTexture(GL_TEXTURE_2D, tx.get_id());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenshot_w, screenshot_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	gl_fbo fbo;
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

	screenshot_with_skin(format(id, suffixes[0]), &*d->skins[0]);
	screenshot_with_skin(format(id, suffixes[1]), &*d->skins[4]);

	// SVG screenshot:
	std::ofstream outf(format(id, suffixes[2]));
	schematic_svg_skin svg_skin(outf, d->active_board->bounding_box());
	render_to(svg_skin);
}

void snygg::reshape(int width, int height) {
	glViewport(0, 0, width, height);
	d->reshaper->reshape(width, height);
	for (size_t i=0; i<d->skins.size(); ++i) {
		d->skins[i]->load_opengl_resources(width, height);
	}

	d->active_skin->set_transformation(d->reshaper->get_transformation());
	d->active_skin->set_pixels_per_unit(d->reshaper->get_pixels_per_unit());
}

void snygg::set_skin_key(scalable_skin* skin) {
	d->active_skin = skin;

	skin->set_transformation(d->reshaper->get_transformation());
	skin->set_pixels_per_unit(d->reshaper->get_pixels_per_unit());
}

void snygg::render_to(skin& s) {
	s.enter_state(skin::board_state);
	d->active_board->render(s);

	s.enter_state(skin::other_state);

	for (auto& item : d->items) item->render(s);
	for (auto& renderable : d->renderables) renderable->render(s);

	s.floor(d->active_board->floor_polygon());
}

void snygg::render() {
	init_gl();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	render_to(*d->active_skin);
}

void snygg::tick_10ms() {
	for (auto& item : d->new_items) {
		d->items.emplace_back(std::move(item));
	}
	d->new_items.clear();

	for (auto& item : d->items) {
		if (!item->is_dead()) item->move();
	}

	std::vector<player*> dead_players;
	for (auto& player : d->players) {
		for (auto& item : d->items) {
			if (!item->is_dead() && player->crashes_with(*item)) item->hit_by(*player);
		}
		if (player->crashes_with(*d->active_board)) {
			dead_players.push_back(&*player);
		}
	}

	for (auto& dead_player : dead_players) dead_player->die();

	auto new_end = std::remove_if(d->items.begin(), d->items.end(), [](const std::unique_ptr<item>& i) { return i->is_dead(); });
	d->items.erase(new_end, d->items.end());
}

void snygg::add_item(std::unique_ptr<item>&& i) {
	d->new_items.emplace_back(std::move(i));
}

void snygg::add_renderable(std::unique_ptr<renderable>&& r) {
	d->renderables.emplace_back(std::move(r));
}
