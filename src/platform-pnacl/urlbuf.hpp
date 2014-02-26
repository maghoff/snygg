#ifndef URLBUF_HPP
#define URLBUF_HPP

#include <streambuf>
#include <string>
#include <vector>
#include <ppapi/cpp/url_loader.h>

class urlbuf : public std::streambuf {
public:
	explicit urlbuf(const std::string& url);
	~urlbuf() override;

private:
	int_type underflow() override;

	std::vector<char> buffer;
	pp::URLLoader urlLoader;
};

#endif
