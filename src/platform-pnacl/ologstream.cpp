#include "ologstream.hpp"

ologstream::ologstream(pp::Instance& instance_, PP_LogLevel log_level) :
	std::ostream(&buf),
	buf(instance_, log_level)
{
}

ologstream::~ologstream() {
	flush();
}
