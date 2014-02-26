#ifndef IURLSTREAM_HPP
#define IURLSTREAM_HPP

#include <istream>
#include <string>
#include "urlbuf.hpp"

namespace pp { class InstanceHandle; }

class iurlstream : public std::istream {
	urlbuf buf;

public:
	iurlstream(pp::InstanceHandle instanceHandle, const std::string& url);
	~iurlstream() override;
};

#endif
