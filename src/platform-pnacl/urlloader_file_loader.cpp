#include "urlloader_file_loader.hpp"
#include <ppapi/cpp/instance_handle.h>
#include "iurlstream.hpp"

urlloader_file_loader::urlloader_file_loader(pp::InstanceHandle instanceHandle_) :
	instanceHandle(instanceHandle_)
{
}

urlloader_file_loader::~urlloader_file_loader() {}

std::unique_ptr<std::istream> urlloader_file_loader::open_file(const std::string& name) {
	return std::unique_ptr<std::istream>(new iurlstream(instanceHandle, "levels/" + name + ".lua"));
}
