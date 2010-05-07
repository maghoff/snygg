#include <iostream>
#include <string>
#include <typeinfo>
#include <ymse/core.hpp>
#include <ymse/core_factory.hpp>
#include "snygg.hpp"
#include "config.hpp"

int main(int argc, const char *argv[])
try {
	std::cout << "CMAKE_INSTALL_PREFIX: " << config::install_prefix << std::endl;

	std::auto_ptr<ymse::core> c(ymse::core_factory());
	c->init(argc, argv);

	std::string board = "levels/wide_screen.lua";
	if (argc >= 2) board = argv[1];

	snygg g(board);

	c->set_game_object(&g);

	c->set_reshaper_object(g.get_reshaper_object());

	c->set_keyboard_handler(g.get_keyboard_handler());

	int ret = c->run();

	c.reset();

	return ret;
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

