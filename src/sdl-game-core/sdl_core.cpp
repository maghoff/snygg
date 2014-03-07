#include <cassert>
#include <map>
#include <stdexcept>
#include <sstream>
#include <SDL.h>
#include <GL/glew.h>
#include "tick_handler.hpp"
#include "keyboard_handler.hpp"
#include "mouse_handler.hpp"
#include "keycodes.hpp"
#include "reshaper.hpp"
#include "sdl_core.hpp"

namespace {
	const int default_width  = 1280;
	const int default_height = 720;
}

namespace game {

const int YSDL_RENDERFRAME = SDL_USEREVENT;

class sdl_error : public std::runtime_error {
public:
	sdl_error(const std::string& w) : std::runtime_error(w) { }
};

void check(bool ok, const std::string& f) {
	if (!ok) {
		std::ostringstream ss;
		ss << f << ": " << SDL_GetError();
		throw sdl_error(ss.str());
	}
}

#define CHECK(f) check(f, #f);

sdl_core::sdl_core() :
	inited(false),
	tick_handler_p(0),
	reshaper_p(0),
	keyboard_handler_p(0),
	mouse_handler_p(0),
	running(true)
{
}

sdl_core::~sdl_core() {
	if (inited) SDL_Quit();
}

void sdl_core::set_video_mode_core(int w, int h, bool fullscreen) {
	unsigned int flags = SDL_OPENGL;

	if (fullscreen) {
		if (!is_fs) {
			windowed_w = screen->w;
			windowed_h = screen->h;
		}
		flags |= SDL_FULLSCREEN;
	} else {
		flags |= SDL_RESIZABLE;
	}

	screen = SDL_SetVideoMode(w, h, 32, flags);
	CHECK(screen);

	glViewport(0, 0, screen->w, screen->h);

	is_fs = fullscreen;
}

void sdl_core::set_video_mode(int w, int h, bool fullscreen) {
	set_video_mode_core(w, h, fullscreen);
	if (reshaper_p) reshaper_p->reshape(screen->w, screen->h);
}

bool sdl_core::is_fullscreen() const {
	return is_fs;
}

void sdl_core::set_fullscreen(bool desired) {
	if (desired != is_fs) toggle_fullscreen();
}

void sdl_core::toggle_fullscreen() {
	if (!is_fs) set_video_mode(desktop_w, desktop_h, true);
	else set_video_mode(windowed_w, windowed_h, false);
}

void sdl_core::init(int argc, char const * const argv[]) {
	CHECK(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != -1);
	inited = true;

	const SDL_VideoInfo* vinf = SDL_GetVideoInfo();
	desktop_w = vinf->current_w;
	desktop_h = vinf->current_h;
	is_fs = false;

	SDL_EnableKeyRepeat(0, 0); //< Just disable it.

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	#if SDL_VERSION_ATLEAST(1,3,0)
	SDL_GL_SetSwapInterval(1);
	#else
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	#endif

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	set_video_mode_core(default_width, default_height, false);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::stringstream ss;
		ss << "GLEW init failed: " << (char*)glewGetErrorString(err) << std::endl;
		throw std::runtime_error(ss.str());
	}
}

void sdl_core::set_cursor_visible(bool show) {
	SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
}

bool sdl_core::get_cursor_visible() const {
	return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
}

void sdl_core::set_tick_handler(game::tick_handler* tick_handler_p_) {
	tick_handler_p = tick_handler_p_;
}

void sdl_core::set_reshaper_object(game::reshaper* reshaper_p_) {
	reshaper_p = reshaper_p_;
}

void sdl_core::set_keyboard_handler(game::keyboard_handler* keyboard_handler_p_) {
	keyboard_handler_p = keyboard_handler_p_;
}

void sdl_core::set_mouse_handler(game::mouse_handler* mouse_handler_p_) {
	mouse_handler_p = mouse_handler_p_;
}

std::map<int, int> key_map = {
	{ SDLK_UP, game::KEY_UP },
	{ SDLK_DOWN, game::KEY_DOWN },
	{ SDLK_LEFT, game::KEY_LEFT },
	{ SDLK_RIGHT, game::KEY_RIGHT },
	{ SDLK_SPACE, game::KEY_SPACE },
	{ SDLK_a, game::KEY_A },
	{ SDLK_b, game::KEY_B },
	{ SDLK_c, game::KEY_C },
	{ SDLK_d, game::KEY_D },
	{ SDLK_e, game::KEY_E },
	{ SDLK_f, game::KEY_F },
	{ SDLK_g, game::KEY_G },
	{ SDLK_h, game::KEY_H },
	{ SDLK_i, game::KEY_I },
	{ SDLK_j, game::KEY_J },
	{ SDLK_k, game::KEY_K },
	{ SDLK_l, game::KEY_L },
	{ SDLK_m, game::KEY_M },
	{ SDLK_n, game::KEY_N },
	{ SDLK_o, game::KEY_O },
	{ SDLK_p, game::KEY_P },
	{ SDLK_q, game::KEY_Q },
	{ SDLK_r, game::KEY_R },
	{ SDLK_s, game::KEY_S },
	{ SDLK_t, game::KEY_T },
	{ SDLK_u, game::KEY_U },
	{ SDLK_v, game::KEY_V },
	{ SDLK_w, game::KEY_W },
	{ SDLK_x, game::KEY_X },
	{ SDLK_y, game::KEY_Y },
	{ SDLK_z, game::KEY_Z },
	{ SDLK_0, game::KEY_0 },
	{ SDLK_1, game::KEY_1 },
	{ SDLK_2, game::KEY_2 },
	{ SDLK_3, game::KEY_3 },
	{ SDLK_4, game::KEY_4 },
	{ SDLK_5, game::KEY_5 },
	{ SDLK_6, game::KEY_6 },
	{ SDLK_7, game::KEY_7 },
	{ SDLK_8, game::KEY_8 },
	{ SDLK_9, game::KEY_9 },
	{ SDLK_LALT, game::KEY_LALT },
	{ SDLK_RALT, game::KEY_RALT },
	{ SDLK_F1, game::KEY_F1 },
	{ SDLK_F2, game::KEY_F2 },
	{ SDLK_F3, game::KEY_F3 },
	{ SDLK_F4, game::KEY_F4 },
	{ SDLK_F5, game::KEY_F5 },
	{ SDLK_F6, game::KEY_F6 },
	{ SDLK_F7, game::KEY_F7 },
	{ SDLK_F8, game::KEY_F8 },
	{ SDLK_F9, game::KEY_F9 },
	{ SDLK_F10, game::KEY_F10 },
	{ SDLK_F11, game::KEY_F11 },
	{ SDLK_F12, game::KEY_F12 },
};

class sdl_timer {
	SDL_TimerID id;

	static Uint32 callback_helper(Uint32 interval, void *th) {
		return reinterpret_cast<sdl_timer*>(th)->callback(interval);
	}

public:
	sdl_timer(int interval) {
		id = SDL_AddTimer(interval, &sdl_timer::callback_helper, this);
		CHECK(id);
	}
	virtual ~sdl_timer() {
		CHECK(SDL_RemoveTimer(id));
	}

	virtual Uint32 callback(Uint32 interval) = 0;
};

class sdl_frame_timer : public sdl_timer {
public:
	volatile bool do_add;

	sdl_frame_timer(int interval) : sdl_timer(interval), do_add(true) { }

	Uint32 callback(Uint32 interval) {
		if (!do_add) return interval;
		do_add = false;

		SDL_Event event;
		SDL_UserEvent userevent;

		userevent.type = YSDL_RENDERFRAME;
		userevent.code = 0;
		userevent.data1 = NULL;
		userevent.data2 = NULL;
		event.type = YSDL_RENDERFRAME;
		event.user = userevent;
		SDL_PushEvent(&event);

		return interval;
	}
};

int sdl_core::run() {
	assert(inited);
	if (!inited) {
		throw std::logic_error(
			"sdl_core::init must be run before std_core::run"
		);
	}

	return_value = 0;

	assert(tick_handler_p);

	if (reshaper_p) reshaper_p->reshape(screen->w, screen->h);

	const unsigned frame_interval = 1000/100;
	sdl_frame_timer sft(frame_interval);

	SDL_Event event;
	unsigned ticks = SDL_GetTicks();
	//unsigned next_frame = ticks + frame_interval;
	while (running) {
		CHECK(SDL_WaitEvent(&event));

		while (ticks < SDL_GetTicks()) {
			unsigned now = SDL_GetTicks();
			assert(now > ticks);
			tick_handler_p->tick(now - ticks);
			ticks = now;
		}

		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;

		case SDL_VIDEORESIZE:
			set_video_mode(event.resize.w, event.resize.h, false);
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if (keyboard_handler_p) {
				std::map<int, int>::const_iterator i = key_map.find(event.key.keysym.sym);
				if (i == key_map.end()) break;
				keyboard_handler_p->key_event(
					i->second,
					event.key.state == SDL_PRESSED
				);
			}
			break;

		case SDL_MOUSEMOTION:
			if (mouse_handler_p) {
				mouse_handler_p->mouse_motion(
					event.motion.xrel, event.motion.yrel,
					event.motion.x, event.motion.y
				);
			}
            break;

		case SDL_MOUSEBUTTONDOWN:
			if (mouse_handler_p) {
				mouse_handler_p->mouse_button_down(
					event.button.button, event.button.x, event.button.y
				);
			}
            break;

		case SDL_MOUSEBUTTONUP:
			if (mouse_handler_p) {
				mouse_handler_p->mouse_button_up(
					event.button.button, event.button.x, event.button.y
				);
			}
            break;

		case YSDL_RENDERFRAME:
			tick_handler_p->render();
			SDL_GL_SwapBuffers();
			sft.do_add = true;
			break;
		}
	}

	return return_value;
}

void sdl_core::stop(int r) {
	return_value = r;
	running = false;
}

}
