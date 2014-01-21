#ifndef YMSE_CORE_FACTORY_HPP
#define YMSE_CORE_FACTORY_HPP

#include <memory>

namespace ymse {

class core;

/// This function chooses a core to instantiate at run time, based on what is
/// available. This is currently stubbed out to just make an SDL core.
std::auto_ptr<core> core_factory();

}

#endif
