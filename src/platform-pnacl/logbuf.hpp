#ifndef LOGBUF_HPP
#define LOGBUF_HPP

#include <streambuf>
#include <vector>
#include <ppapi/cpp/instance.h>

class logbuf : public std::streambuf {
	pp::Instance& instance;
	PP_LogLevel log_level;
	std::vector<char> buffer;

	int_type overflow(int_type) override;
	int sync() override;

public:
	logbuf(pp::Instance&, PP_LogLevel);
	~logbuf() override;
};

#endif
