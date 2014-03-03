#ifndef SNYGG_INSTANCE_HPP
#define SNYGG_INSTANCE_HPP

#include <thread>
#include <ppapi/cpp/instance.h>

class snygg_instance : public pp::Instance {
	std::thread game_thread;

public:
	using pp::Instance::Instance;
    ~snygg_instance() override;

	bool Init(uint32_t argc, const char* argn[], const char* argv[]) override;
};

#endif
