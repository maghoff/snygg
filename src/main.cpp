#include <iostream>
#include <ymse/glut_core.hpp>

int main(int argc, const char ** argv)
try {
	ymse::glut_core core;

	core.init(argc, argv);

	return 0;
}
catch(...) {
	std::cerr << "Unknown error. Terminating" << std::endl;
	return -1;
}

