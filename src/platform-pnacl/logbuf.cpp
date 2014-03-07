#include "logbuf.hpp"
#include <ppapi/cpp/var.h>

logbuf::logbuf(pp::Instance& instance_, PP_LogLevel log_level_) :
	instance(instance_),
	log_level(log_level_),
	buffer(4096)
{
	setp(buffer.data(), buffer.data() + buffer.size());
}

logbuf::~logbuf() { }

logbuf::int_type logbuf::overflow(logbuf::int_type c) {
	sync();
	return sputc(c);
}

int logbuf::sync() {
	auto begin = pbase(), end = pptr();
	if (begin != end && *(end-1) == '\n') --end;
	if (begin != end) instance.LogToConsole(log_level, pp::Var(std::string(begin, end)));
	setp(buffer.data(), buffer.data() + buffer.size());
	return 0;
}
