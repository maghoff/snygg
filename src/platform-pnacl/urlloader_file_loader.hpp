#ifndef URLLOADER_FILE_LOADER_HPP
#define URLLOADER_FILE_LOADER_HPP

#include <ppapi/cpp/instance_handle.h>
#include "file_loader.hpp"

class urlloader_file_loader : public file_loader {
	pp::InstanceHandle instanceHandle;

public:
	explicit urlloader_file_loader(pp::InstanceHandle instanceHandle);
	~urlloader_file_loader() override;

	std::unique_ptr<std::istream> open_file(const std::string&) override;
};

#endif
