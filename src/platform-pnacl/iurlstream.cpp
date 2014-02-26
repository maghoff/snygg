#include "iurlstream.hpp"
#include <ppapi/cpp/instance_handle.h>

iurlstream::iurlstream(pp::InstanceHandle instanceHandle, const std::string& url) :
	buf(instanceHandle, url)
{
	rdbuf(&buf);
}

iurlstream::~iurlstream() {}
