#include "filesystem_file_loader.hpp"
#include <fstream>
#include <sstream>

void filesystem_file_loader::add_search_path(boost::filesystem::path path) {
	search_paths.emplace_back(std::move(path));
}

std::unique_ptr<std::istream> filesystem_file_loader::open_file(const std::string& name) {
	std::unique_ptr<std::ifstream> in(new std::ifstream);

	in->open(name);
	if (*in) return std::move(in);

	for (auto& search_path : search_paths) {
		in->open((search_path / name).string() + ".lua");
		if (*in) return std::move(in);
	}

	std::stringstream ss;
	ss << "Could not find module \"" << name << "\" in search path [";
	bool first = true;
	for (auto& path : search_paths) {
		if (first) first = false;
		else ss << ", ";
		ss << path;
	}
	ss << "]";

	throw file_not_found(ss.str());
}
