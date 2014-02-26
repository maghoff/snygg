#ifndef URLBUF_HPP
#define URLBUF_HPP

#include <streambuf>
#include <string>
#include <vector>
#include <ppapi/cpp/url_loader.h>

namespace pp { class InstanceHandle; }

class urlbuf : public std::streambuf {
	int_type underflow() override;

	std::vector<char> buffer;
	pp::URLLoader urlLoader;

public:
	urlbuf(pp::InstanceHandle instanceHandle, const std::string& url);
	~urlbuf() override;
};

#endif
