#include <SDL_main.h>
#include <iostream>
#include <string>
#include <typeinfo>
#include <ymse/sdl_core.hpp>
#include "paths.hpp"
#include "snygg.hpp"

int main(int argc, char *argv[])
try {
	paths::set_argv_zero(argv[0]);

	ymse::sdl_core core;
	core.init(argc, const_cast<const char**>(argv));

	std::string board = "wide_screen.lua";
	if (argc >= 2) board = argv[1];

	snygg g(board);
	g.attach_to_core(core);

	return core.run();
}
catch (const std::exception& e) {
	std::cerr <<
		"Fatal error: " << e.what() << "\n"
		"Typeid: " << typeid(e).name() << std::endl;
	return -1;
}
catch (...) {
	std::cerr << "Unknown fatal error" << std::endl;
	return -1;
}; //< ; is a workaround for MSVC

