#include "snygg_module.hpp"
#include <ppapi/gles2/gl2ext_ppapi.h>
#include "snygg_instance.hpp"

snygg_module::~snygg_module() {
	glTerminatePPAPI();
}

bool snygg_module::Init() {
	return glInitializePPAPI(get_browser_interface()) == 1;
}

pp::Instance* snygg_module::CreateInstance(PP_Instance pp_instance) {
	return new snygg_instance(pp_instance);
}
