#include "core_factory.hpp"
#include "sdl_core.hpp"

namespace ymse {

std::unique_ptr<game::core> core_factory() {
	return std::unique_ptr<game::core>(new sdl_core);
}

}
