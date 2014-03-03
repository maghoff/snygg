#include <ppapi/cpp/module.h>
#include "snygg_module.hpp"

namespace pp {
	Module* CreateModule() {
		return new snygg_module();
	}
}
