#include <ymse/gl.h>
#include <SDL_main.h>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <typeinfo>
#include <ymse/sdl_core.hpp>
#include "paths.hpp"
#include "snygg.hpp"

int main(int argc, char *argv[])
try {
	paths::set_argv_zero(argv[0]);

	ymse::sdl_core core;
	core.init(argc, argv);
	core.set_cursor_visible(false);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::stringstream ss;
		ss << "GLEW init failed: " << (char*)glewGetErrorString(err) << std::endl;
		throw std::runtime_error(ss.str());
	}

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

