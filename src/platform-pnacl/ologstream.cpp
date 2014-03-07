#include "ologstream.hpp"

ologstream::ologstream(pp::Instance& instance, PP_LogLevel log_level) :
	buf(instance, log_level)
{
	rdbuf(&buf);
}

ologstream::~ologstream()
{
}
