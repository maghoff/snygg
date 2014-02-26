#include "urlbuf.hpp"
#include <stdexcept>
#include <ppapi_simple/ps_instance.h>
#include <ppapi/cpp/var.h>
#include <ppapi/cpp/url_request_info.h>

urlbuf::urlbuf(const std::string& url) :
	buffer(4096)
{
	auto instanceHandle = pp::InstanceHandle(PSInstance::GetInstance()); //< Should be injected dependency

	urlLoader = pp::URLLoader(instanceHandle);

	pp::URLRequestInfo urlRequestInfo(instanceHandle);
	urlRequestInfo.SetURL(url);

	auto result = urlLoader.Open(urlRequestInfo, pp::BlockUntilComplete());

	if (result != PP_OK) throw std::runtime_error("urlLoader.Open returned an error code");
}

urlbuf::~urlbuf() { }

urlbuf::int_type urlbuf::underflow() {
	auto bytesRead = urlLoader.ReadResponseBody(buffer.data(), buffer.size(), pp::BlockUntilComplete());
	if (bytesRead < 0) throw std::runtime_error("urlLoader.ReadResponseBody returned an error code");

	setg(buffer.data(), buffer.data(), buffer.data() + bytesRead);

	return bytesRead ? traits_type::to_int_type(buffer[0]) : traits_type::eof();
}
