#include "core_factory.hpp"
#include "sdl_core.hpp"

namespace ymse {

std::auto_ptr<core> core_factory() {
	return std::auto_ptr<core>(new sdl_core);
}

}
