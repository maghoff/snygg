#include "nacl_errors.hpp"
#include <sstream>
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/instance.h>
#include <ppapi/cpp/var.h>

void fail(err error_code, const std::string& message) {
	std::stringstream ss;
	ss << "A fatal error occurred: " << message << " (error code " << static_cast<int>(error_code) << ')';

	auto module = pp::Module::Get();

	// The following assumes only one instance:
	auto instance = module->current_instances().begin()->second;

	instance->LogToConsole(PP_LOGLEVEL_ERROR, pp::Var(ss.str()));

	exit(static_cast<int>(error_code));
}
