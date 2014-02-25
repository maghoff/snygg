#include "urlloader_file_loader.hpp"
#include "iurlstream.hpp"

std::unique_ptr<std::istream> urlloader_file_loader::open_file(const std::string& name) {
	return std::unique_ptr<std::istream>(new iurlstream("levels/" + name + ".lua"));
}
