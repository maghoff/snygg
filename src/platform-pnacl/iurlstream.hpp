#ifndef IURLSTREAM_HPP
#define IURLSTREAM_HPP

#include <istream>
#include <string>
#include "urlbuf.hpp"

class iurlstream : public std::istream {
	urlbuf buf;

public:
	iurlstream(const std::string& url);
	~iurlstream() override;
};

#endif
