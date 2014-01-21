#include "core_factory.hpp"
#include "sdl_core.hpp"

namespace ymse {

std::unique_ptr<core> core_factory() {
	return std::unique_ptr<core>(new sdl_core);
}

}
