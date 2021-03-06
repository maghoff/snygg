#ifndef SNYGG_INSTANCE_HPP
#define SNYGG_INSTANCE_HPP

#include <thread>
#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/graphics_3d_client.h>
#include <ppapi/cpp/graphics_3d.h>
#include <ppapi/cpp/core.h>
#include <box_reshaper.hpp>
#include <item_container.hpp>
#include <player.hpp>
#include <movable.hpp>
#include <score_listener.hpp>
#include <renderable.hpp>
#include <bindable_keyboard_handler.hpp>
#include <surface.hpp>
#include "renderable_complex_polygon.hpp"
#include "renderable_recording_draw_elements.hpp"
#include "ologstream.hpp"

class board_provider;
class board;
class food_generator;
class buffering_skin;
class metaballs;

namespace pp {
	class KeyboardInputEvent;
}

class snygg_instance : public pp::Instance, pp::Graphics3DClient, item_container, score_listener {
	ologstream lout, lerr;

	std::thread load_board_thread, load_resources_thread;
	std::shared_ptr<board_provider> bpp;
	std::shared_ptr<board> bp;
	std::string current_board_name, loading_board_name;
	std::map<std::string, std::vector<char>> resources;
	bool resources_loaded = false;
	std::thread load_images_thread;
	std::map<std::string, image::surface> images;
	bool images_loaded = false;

	bool ready = false;

	pp::Graphics3D context;

	renderable_complex_polygon floor;
	renderable_recording_draw_elements walls;

	std::unique_ptr<buffering_skin> skin;
	std::unique_ptr<metaballs> metaballs;

    int storedWidth=0, storedHeight=0;
	game::box_reshaper reshaper;

	double startTime;
	int ticks;
	std::vector<std::unique_ptr<renderable>> renderables;
	std::vector<std::unique_ptr<crashable>> crashables, new_crashables;
	std::vector<std::unique_ptr<movable>> movables;
	std::vector<std::unique_ptr<player>> players;
	std::unique_ptr<food_generator> fg;
	void add_renderable(std::unique_ptr<renderable>&&) override;
	void add_crashable(std::unique_ptr<crashable>&&) override;
	void add_movable(std::unique_ptr<movable>&&) override;
	void tick_10ms();
	void simulate_until(PP_TimeTicks);

	void score_updated(int current_score) override;
	void died(int final_score) override;

	std::unordered_map<int, bool> key_states;
	game::bindable_keyboard_handler kbd;
	bool handle_key_event(const pp::KeyboardInputEvent& event);
	bool handle_keypress_event(const pp::KeyboardInputEvent& event);
	bool HandleInputEvent(const pp::InputEvent& event) override;

	void load_board(const std::string&);

	void HandleMessage(const pp::Var& var_message) override;

	void Graphics3DContextLost() override;
	bool render_callback_pending = false, should_render = false;
	friend void render_callback_trampoline(void* userdata, int32_t status);
	void render_callback(int32_t status);
	void render();

	void maybe_ready();
	void update_walls();
	void check_gl_error();

	void DidChangeView(const pp::View&) override;

public:
	explicit snygg_instance(PP_Instance);
	~snygg_instance() override;

	bool Init(uint32_t argc, const char* argn[], const char* argv[]) override;
};

#endif
