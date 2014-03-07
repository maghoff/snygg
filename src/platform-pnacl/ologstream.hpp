#ifndef OLOGSTREAM_HPP
#define OLOGSTREAM_HPP

#include <ostream>
#include "logbuf.hpp"

class ologstream : public std::ostream {
	logbuf buf;

public:
	ologstream(pp::Instance&, PP_LogLevel);
    ~ologstream() override;
};

#endif
