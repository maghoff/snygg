#ifndef URLLOADER_FILE_LOADER_HPP
#define URLLOADER_FILE_LOADER_HPP

#include "file_loader.hpp"

class urlloader_file_loader : public file_loader {
public:
	std::unique_ptr<std::istream> open_file(const std::string&) override;
};

#endif
