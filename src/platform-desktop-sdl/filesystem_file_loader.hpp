#ifndef SNYGG_FILESYSTEM_FILE_LOADER_HPP
#define SNYGG_FILESYSTEM_FILE_LOADER_HPP

#include <vector>
#include <boost/filesystem/path.hpp>
#include "file_loader.hpp"

class filesystem_file_loader : public file_loader {
	std::vector<boost::filesystem::path> search_paths;

public:
	void add_search_path(boost::filesystem::path);

	std::unique_ptr<std::istream> open_file(const std::string&) override;
};

#endif
