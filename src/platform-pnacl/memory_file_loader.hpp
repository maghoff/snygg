#ifndef MEMORY_FILE_LOADER_HPP
#define MEMORY_FILE_LOADER_HPP

#include "file_loader.hpp"

class memory_file_loader : public file_loader {
public:
	std::unique_ptr<std::istream> open_file(const std::string&) override;
};

#endif
