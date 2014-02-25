#include "iurlstream.hpp"

iurlstream::iurlstream(const std::string& url) : buf(url) {
	rdbuf(&buf);
}

iurlstream::~iurlstream() {}
