#ifndef URLBUF_HPP
#define URLBUF_HPP

#include <streambuf>
#include <string>
#include <vector>
#include <ppapi/c/ppb_url_loader.h>
#include <ppapi/c/pp_resource.h>

class urlbuf : public std::streambuf {
public:
	explicit urlbuf(const std::string& url);
	~urlbuf();

private:
	int_type underflow() override;

	std::vector<char> buffer;

    const PPB_URLLoader* urlLoaderInterface;
    PP_Resource urlLoader;
};

#endif
