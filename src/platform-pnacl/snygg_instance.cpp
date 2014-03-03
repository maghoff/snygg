#include "snygg_instance.hpp"

#include <ppapi/cpp/var.h>
#include <sstream>
#include <fileutil.hpp>
#include <lua_board_provider.hpp>
#include <board.hpp>
#include <schematic_svg_skin.hpp>
#include <rect.hpp>
#include "urlloader_file_loader.hpp"

void render_board(pp::InstanceHandle instanceHandle, const std::string& boardname) {
	urlloader_file_loader loader(instanceHandle);

	lua_board_provider board_provider(loader, boardname.length() ? boardname : "wide_screen");
	board b(board_provider);

	std::stringstream svgstream;
	{
		schematic_svg_skin svg_skin(svgstream, b.bounding_box());
		svg_skin.enter_state(skin::board_state);
		b.render(svg_skin);
		svg_skin.enter_state(skin::other_state);
		svg_skin.floor(b.floor_polygon());
	}

	pp::Instance instance{instanceHandle.pp_instance()};
	instance.PostMessage(pp::Var(svgstream.str()));
}


snygg_instance::~snygg_instance() {
	game_thread.join();
}

bool snygg_instance::Init(uint32_t argc, const char* argn[], const char* argv[]) {
	std::map<std::string, std::string> args;
	for (int i=0; i<argc; ++i) args[argn[i]] = argv[i];

	game_thread = std::thread(&render_board, pp::InstanceHandle(this), args["board"]);

	return true;
}
