#include <cassert>
#include <map>
#include <stdexcept>
#include <sstream>
#include <boost/assign.hpp>
#include <SDL.h>
#include "gl.h"
#include "game.hpp"
#include "keyboard_handler.hpp"
#include "mouse_handler.hpp"
#include "keycodes.hpp"
#include "reshaper.hpp"
#include "sdl_core.hpp"

namespace {
	const int default_width  = 800;
	const int default_height = 600;
}

namespace ymse {

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
	game_p(0),
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
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
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

void sdl_core::set_game_object(game* game_p_) {
	game_p = game_p_;
}

void sdl_core::set_reshaper_object(reshaper* reshaper_p_) {
	reshaper_p = reshaper_p_;
}

void sdl_core::set_keyboard_handler(keyboard_handler* keyboard_handler_p_) {
	keyboard_handler_p = keyboard_handler_p_;
}

void sdl_core::set_mouse_handler(mouse_handler* mouse_handler_p_) {
	mouse_handler_p = mouse_handler_p_;
}

std::map<int, int> key_map = boost::assign::map_list_of<int, int>
	(SDLK_UP, ymse::KEY_UP)
	(SDLK_DOWN, ymse::KEY_DOWN)
	(SDLK_LEFT, ymse::KEY_LEFT)
	(SDLK_RIGHT, ymse::KEY_RIGHT)
	(SDLK_SPACE, ymse::KEY_SPACE)
	(SDLK_a, ymse::KEY_A)
	(SDLK_b, ymse::KEY_B)
	(SDLK_c, ymse::KEY_C)
	(SDLK_d, ymse::KEY_D)
	(SDLK_e, ymse::KEY_E)
	(SDLK_f, ymse::KEY_F)
	(SDLK_g, ymse::KEY_G)
	(SDLK_h, ymse::KEY_H)
	(SDLK_i, ymse::KEY_I)
	(SDLK_j, ymse::KEY_J)
	(SDLK_k, ymse::KEY_K)
	(SDLK_l, ymse::KEY_L)
	(SDLK_m, ymse::KEY_M)
	(SDLK_n, ymse::KEY_N)
	(SDLK_o, ymse::KEY_O)
	(SDLK_p, ymse::KEY_P)
	(SDLK_q, ymse::KEY_Q)
	(SDLK_r, ymse::KEY_R)
	(SDLK_s, ymse::KEY_S)
	(SDLK_t, ymse::KEY_T)
	(SDLK_u, ymse::KEY_U)
	(SDLK_v, ymse::KEY_V)
	(SDLK_w, ymse::KEY_W)
	(SDLK_x, ymse::KEY_X)
	(SDLK_y, ymse::KEY_Y)
	(SDLK_z, ymse::KEY_Z)
	(SDLK_0, ymse::KEY_0)
	(SDLK_1, ymse::KEY_1)
	(SDLK_2, ymse::KEY_2)
	(SDLK_3, ymse::KEY_3)
	(SDLK_4, ymse::KEY_4)
	(SDLK_5, ymse::KEY_5)
	(SDLK_6, ymse::KEY_6)
	(SDLK_7, ymse::KEY_7)
	(SDLK_8, ymse::KEY_8)
	(SDLK_9, ymse::KEY_9)
	(SDLK_LALT, ymse::KEY_LALT)
	(SDLK_RALT, ymse::KEY_RALT)
	(SDLK_F1, ymse::KEY_F1)
	(SDLK_F2, ymse::KEY_F2)
	(SDLK_F3, ymse::KEY_F3)
	(SDLK_F4, ymse::KEY_F4)
	(SDLK_F5, ymse::KEY_F5)
	(SDLK_F6, ymse::KEY_F6)
	(SDLK_F7, ymse::KEY_F7)
	(SDLK_F8, ymse::KEY_F8)
	(SDLK_F9, ymse::KEY_F9)
	(SDLK_F10, ymse::KEY_F10)
	(SDLK_F11, ymse::KEY_F11)
	(SDLK_F12, ymse::KEY_F12)
;

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

	assert(game_p);

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
			game_p->tick(now - ticks);
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
			game_p->render();
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
