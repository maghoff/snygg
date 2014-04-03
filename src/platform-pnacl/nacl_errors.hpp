#ifndef NACL_ERRORS_HPP
#define NACL_ERRORS_HPP

// Error reporting for nacl, which doesn't handle exceptions, the poor thing

#include <string>

enum class err : int {
	graphics3d_context_lost = 10,
	unable_to_create_3d_context,
	unable_to_bind_3d_context,
	urlloader_open,
	urlloader_readresponsebody
};

void fail(err, const std::string&);

#endif
