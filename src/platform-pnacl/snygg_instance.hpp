#ifndef SNYGG_INSTANCE_HPP
#define SNYGG_INSTANCE_HPP

#include <thread>
#include <memory>
#include <functional>
#include <vector>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/graphics_3d_client.h>
#include <ppapi/cpp/graphics_3d.h>
#include <box_reshaper.hpp>
#include "renderable_complex_polygon.hpp"
#include "buffering_skin.hpp"
#include "ologstream.hpp"

class board;

class snygg_instance : public pp::Instance, pp::Graphics3DClient {
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
