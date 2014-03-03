#ifndef SNYGG_MODULE_HPP
#define SNYGG_MODULE_HPP

#include <ppapi/cpp/module.h>

class snygg_module : public pp::Module {
public:
	~snygg_module() override;

	bool Init() override;
	pp::Instance* CreateInstance(PP_Instance) override;
};

#endif
