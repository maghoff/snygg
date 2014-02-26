#include <typeinfo>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <fileutil.hpp>
#include <lua_board_provider.hpp>
#include <board.hpp>
#include <schematic_svg_skin.hpp>
#include <rect.hpp>
#include "urlloader_file_loader.hpp"

#include <ppapi_simple/ps_instance.h>
#include <ppapi_simple/ps_main.h>

int platform_nacl_main(int argc, char* argv[])
try {
	urlloader_file_loader loader{pp::InstanceHandle(PSInstance::GetInstance())};

	lua_board_provider board_provider(loader, argc >= 1 ? argv[0] : "wide_screen");
	board b(board_provider);

	{
		schematic_svg_skin svg_skin(std::cout, b.bounding_box());
		svg_skin.enter_state(skin::board_state);
		b.render(svg_skin);
		svg_skin.enter_state(skin::other_state);
		svg_skin.floor(b.floor_polygon());
	}
	std::cout.flush();

	sleep(5);

	return 0;
}
catch (std::exception& err) {
	std::cerr << "Exception of type " << typeid(err).name() << std::endl;
	std::cerr << err.what() << std::endl;
	sleep(2); //< Give ppapi a chance to catch up. Sigh.
	return 1;
}

PPAPI_SIMPLE_REGISTER_MAIN(platform_nacl_main)
