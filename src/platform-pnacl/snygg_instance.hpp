#ifndef SNYGG_INSTANCE_HPP
#define SNYGG_INSTANCE_HPP

#include <thread>
#include <memory>
#include <functional>
#include <vector>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/graphics_3d_client.h>
#include <ppapi/cpp/graphics_3d.h>
#include <ppapi/cpp/core.h>
#include <box_reshaper.hpp>
#include <item_container.hpp>
#include <item.hpp>
#include <renderable.hpp>
#include "renderable_complex_polygon.hpp"
#include "buffering_skin.hpp"
#include "ologstream.hpp"

class board;

class snygg_instance : public pp::Instance, pp::Graphics3DClient, item_container {
	ologstream lout;

	std::thread load_board_thread, load_resources_thread;
	std::shared_ptr<board> bp;
	std::map<std::string, std::vector<char>> resources;
	bool resources_loaded = false;

	pp::Graphics3D context;
	std::shared_ptr<std::function<void(void*)>> doRender;

	int floorProgram, colorProgram;
	renderable_complex_polygon floor;

	buffering_skin skin;

	game::box_reshaper reshaper;

	double startTime;
	int ticks;
	std::vector<std::unique_ptr<item>> items;
	std::vector<std::unique_ptr<renderable>> renderables;
	void add_item(std::unique_ptr<item>&&) override;
	void add_renderable(std::unique_ptr<renderable>&&) override;
	void simulate_until(PP_TimeTicks);

	void Graphics3DContextLost() override;
	void render(void*);
	void maybe_ready();
	void check_gl_error();

	void DidChangeView(const pp::View&) override;

public:
	explicit snygg_instance(PP_Instance);
	~snygg_instance() override;

	bool Init(uint32_t argc, const char* argn[], const char* argv[]) override;
};

#endif
