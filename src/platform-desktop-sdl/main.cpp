#include <SDL_main.h>
#include <lean_windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <sdl_core.hpp>
#include "paths.hpp"
#include "snygg.hpp"

int main(int argc, char *argv[])
try {
	paths::set_argv_zero(argv[0]);

	game::sdl_core core;
	core.init(argc, argv);
	core.set_cursor_visible(false);

	std::string board = "wide_screen.lua";
	if (argc >= 2) board = argv[1];

	snygg g(board);
	g.attach_to_core(core);

	return core.run();
}
catch (const std::exception& e) {
	std::ostringstream ss;
	ss <<
		"Fatal error: " << e.what() << "\n"
		"Typeid: " << typeid(e).name();
	std::cerr << ss.str() << std::endl;

#ifdef _WIN32
		MessageBox(0, ss.str().c_str(), "Fatal error", MB_OK | MB_ICONERROR);
#endif

	return -1;
}
catch (...) {
	std::cerr << "Unknown fatal error" << std::endl;

#ifdef _WIN32
		MessageBox(0, "Unknown fatal error", "Fatal error", MB_OK | MB_ICONERROR);
#endif

	return -1;
}; //< ; is a workaround for MSVC
