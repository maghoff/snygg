#include "logbuf.hpp"
#include <algorithm>
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

	for (auto newline = std::find(begin, end, '\n'); newline != end; newline = std::find(begin, end, '\n')) {
		instance.LogToConsole(log_level, pp::Var(std::string(begin, newline)));
		begin = newline + 1;
	}

	if (begin != end) {
		if (begin != buffer.data()) {
			auto end_of_copy = std::copy(begin, end, buffer.data());
			setp(end_of_copy, buffer.data() + buffer.size());
		} else {
			instance.LogToConsole(log_level, pp::Var(std::string(begin, end)));
			setp(buffer.data(), buffer.data() + buffer.size());
		}
	} else {
		setp(buffer.data(), buffer.data() + buffer.size());
	}

	return 0;
}
