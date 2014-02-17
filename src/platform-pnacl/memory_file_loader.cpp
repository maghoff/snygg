#include "memory_file_loader.hpp"
#include <map>
#include <sstream>

extern std::map<std::string, std::string> modules;

std::unique_ptr<std::istream> memory_file_loader::open_file(const std::string& name) {
	auto it = modules.find(name);

	if (it != modules.end()) {
		return std::unique_ptr<std::istream>(new std::stringstream(it->second));
	}

	std::stringstream ss;
	ss << "Could not find module \"" << name << "\" in in-memory modules [";
	bool first = true;
	for (auto& module : modules) {
		if (first) first = false;
		else ss << ", ";
		ss << module.first;
	}
	ss << "]";

	throw file_not_found(ss.str());
}


unsigned char levels_util_lua[] = {
  0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x75, 0x74, 0x69,
  0x6c, 0x5f, 0x68, 0x65, 0x61, 0x70, 0x28, 0x29, 0x0a, 0x09, 0x6c, 0x6f,
  0x63, 0x61, 0x6c, 0x20, 0x70, 0x69, 0x20, 0x3d, 0x20, 0x6d, 0x61, 0x74,
  0x68, 0x2e, 0x70, 0x69, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20,
  0x73, 0x65, 0x67, 0x6d, 0x65, 0x6e, 0x74, 0x5f, 0x68, 0x65, 0x61, 0x70,
  0x20, 0x3d, 0x20, 0x73, 0x65, 0x67, 0x6d, 0x65, 0x6e, 0x74, 0x5f, 0x68,
  0x65, 0x61, 0x70, 0x28, 0x29, 0x0a, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61,
  0x6c, 0x20, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x74,
  0x75, 0x72, 0x74, 0x6c, 0x65, 0x28, 0x68, 0x65, 0x61, 0x70, 0x2c, 0x20,
  0x78, 0x2c, 0x20, 0x79, 0x2c, 0x20, 0x64, 0x78, 0x2c, 0x20, 0x64, 0x79,
  0x29, 0x0a, 0x09, 0x09, 0x66, 0x6f, 0x72, 0x77, 0x61, 0x72, 0x64, 0x20,
  0x3d, 0x20, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x28, 0x6c,
  0x65, 0x6e, 0x29, 0x0a, 0x09, 0x09, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c,
  0x20, 0x78, 0x32, 0x2c, 0x20, 0x79, 0x32, 0x20, 0x3d, 0x20, 0x78, 0x20,
  0x2b, 0x20, 0x64, 0x78, 0x2a, 0x6c, 0x65, 0x6e, 0x2c, 0x20, 0x79, 0x20,
  0x2b, 0x20, 0x64, 0x79, 0x2a, 0x6c, 0x65, 0x6e, 0x0a, 0x09, 0x09, 0x09,
  0x68, 0x65, 0x61, 0x70, 0x3a, 0x6c, 0x69, 0x6e, 0x65, 0x28, 0x78, 0x2c,
  0x20, 0x79, 0x2c, 0x20, 0x78, 0x32, 0x2c, 0x20, 0x79, 0x32, 0x29, 0x0a,
  0x09, 0x09, 0x09, 0x78, 0x2c, 0x20, 0x79, 0x20, 0x3d, 0x20, 0x78, 0x32,
  0x2c, 0x20, 0x79, 0x32, 0x0a, 0x09, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x0a,
  0x09, 0x09, 0x6c, 0x65, 0x66, 0x74, 0x20, 0x3d, 0x20, 0x66, 0x75, 0x6e,
  0x63, 0x74, 0x69, 0x6f, 0x6e, 0x28, 0x72, 0x2c, 0x20, 0x61, 0x6e, 0x67,
  0x29, 0x0a, 0x09, 0x09, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x6e,
  0x78, 0x2c, 0x20, 0x6e, 0x79, 0x20, 0x3d, 0x20, 0x2d, 0x64, 0x79, 0x2c,
  0x20, 0x64, 0x78, 0x0a, 0x09, 0x09, 0x09, 0x69, 0x66, 0x20, 0x61, 0x6e,
  0x67, 0x20, 0x3c, 0x20, 0x30, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x20, 0x6e,
  0x78, 0x2c, 0x20, 0x6e, 0x79, 0x20, 0x3d, 0x20, 0x2d, 0x6e, 0x78, 0x2c,
  0x20, 0x2d, 0x6e, 0x79, 0x20, 0x65, 0x6e, 0x64, 0x0a, 0x09, 0x09, 0x09,
  0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x63, 0x78, 0x2c, 0x20, 0x63, 0x79,
  0x20, 0x3d, 0x20, 0x78, 0x2b, 0x6e, 0x78, 0x2a, 0x72, 0x2c, 0x20, 0x79,
  0x2b, 0x6e, 0x79, 0x2a, 0x72, 0x0a, 0x09, 0x09, 0x09, 0x6c, 0x6f, 0x63,
  0x61, 0x6c, 0x20, 0x73, 0x74, 0x61, 0x72, 0x74, 0x5f, 0x61, 0x6e, 0x67,
  0x20, 0x3d, 0x20, 0x6d, 0x61, 0x74, 0x68, 0x2e, 0x61, 0x74, 0x61, 0x6e,
  0x32, 0x28, 0x2d, 0x6e, 0x79, 0x2c, 0x20, 0x2d, 0x6e, 0x78, 0x29, 0x0a,
  0x09, 0x09, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x65, 0x6e, 0x64,
  0x5f, 0x61, 0x6e, 0x67, 0x20, 0x3d, 0x20, 0x73, 0x74, 0x61, 0x72, 0x74,
  0x5f, 0x61, 0x6e, 0x67, 0x20, 0x2b, 0x20, 0x61, 0x6e, 0x67, 0x0a, 0x09,
  0x09, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x61, 0x72, 0x63, 0x28, 0x63,
  0x78, 0x2c, 0x20, 0x63, 0x79, 0x2c, 0x20, 0x72, 0x2c, 0x20, 0x73, 0x74,
  0x61, 0x72, 0x74, 0x5f, 0x61, 0x6e, 0x67, 0x2c, 0x20, 0x65, 0x6e, 0x64,
  0x5f, 0x61, 0x6e, 0x67, 0x29, 0x0a, 0x09, 0x09, 0x09, 0x6c, 0x6f, 0x63,
  0x61, 0x6c, 0x20, 0x6e, 0x78, 0x32, 0x2c, 0x20, 0x6e, 0x79, 0x32, 0x20,
  0x3d, 0x20, 0x6d, 0x61, 0x74, 0x68, 0x2e, 0x63, 0x6f, 0x73, 0x28, 0x65,
  0x6e, 0x64, 0x5f, 0x61, 0x6e, 0x67, 0x29, 0x2c, 0x20, 0x6d, 0x61, 0x74,
  0x68, 0x2e, 0x73, 0x69, 0x6e, 0x28, 0x65, 0x6e, 0x64, 0x5f, 0x61, 0x6e,
  0x67, 0x29, 0x0a, 0x09, 0x09, 0x09, 0x64, 0x78, 0x2c, 0x20, 0x64, 0x79,
  0x20, 0x3d, 0x20, 0x2d, 0x6e, 0x79, 0x32, 0x2c, 0x20, 0x6e, 0x78, 0x32,
  0x0a, 0x09, 0x09, 0x09, 0x69, 0x66, 0x20, 0x61, 0x6e, 0x67, 0x20, 0x3c,
  0x20, 0x30, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x20, 0x64, 0x78, 0x2c, 0x20,
  0x64, 0x79, 0x20, 0x3d, 0x20, 0x2d, 0x64, 0x78, 0x2c, 0x20, 0x2d, 0x64,
  0x79, 0x20, 0x65, 0x6e, 0x64, 0x0a, 0x09, 0x09, 0x09, 0x78, 0x2c, 0x20,
  0x79, 0x20, 0x3d, 0x20, 0x63, 0x78, 0x2b, 0x6e, 0x78, 0x32, 0x2a, 0x72,
  0x2c, 0x20, 0x63, 0x79, 0x2b, 0x6e, 0x79, 0x32, 0x2a, 0x72, 0x0a, 0x09,
  0x09, 0x65, 0x6e, 0x64, 0x0a, 0x0a, 0x09, 0x09, 0x72, 0x69, 0x67, 0x68,
  0x74, 0x20, 0x3d, 0x20, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e,
  0x28, 0x72, 0x2c, 0x20, 0x61, 0x6e, 0x67, 0x29, 0x0a, 0x09, 0x09, 0x09,
  0x6c, 0x65, 0x66, 0x74, 0x28, 0x72, 0x2c, 0x20, 0x2d, 0x61, 0x6e, 0x67,
  0x29, 0x0a, 0x09, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x0a, 0x09, 0x09, 0x72,
  0x65, 0x74, 0x75, 0x72, 0x6e, 0x20, 0x7b, 0x0a, 0x09, 0x09, 0x09, 0x66,
  0x6f, 0x72, 0x77, 0x61, 0x72, 0x64, 0x20, 0x3d, 0x20, 0x66, 0x6f, 0x72,
  0x77, 0x61, 0x72, 0x64, 0x2c, 0x0a, 0x09, 0x09, 0x09, 0x6c, 0x65, 0x66,
  0x74, 0x20, 0x3d, 0x20, 0x6c, 0x65, 0x66, 0x74, 0x2c, 0x0a, 0x09, 0x09,
  0x09, 0x72, 0x69, 0x67, 0x68, 0x74, 0x20, 0x3d, 0x20, 0x72, 0x69, 0x67,
  0x68, 0x74, 0x0a, 0x09, 0x09, 0x7d, 0x0a, 0x09, 0x65, 0x6e, 0x64, 0x0a,
  0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x66, 0x75, 0x6e, 0x63,
  0x74, 0x69, 0x6f, 0x6e, 0x20, 0x62, 0x6f, 0x78, 0x28, 0x68, 0x65, 0x61,
  0x70, 0x2c, 0x20, 0x78, 0x31, 0x2c, 0x20, 0x79, 0x31, 0x2c, 0x20, 0x78,
  0x32, 0x2c, 0x20, 0x79, 0x32, 0x2c, 0x20, 0x73, 0x72, 0x29, 0x0a, 0x09,
  0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x77, 0x2c, 0x20, 0x68, 0x0a,
  0x0a, 0x09, 0x09, 0x77, 0x20, 0x3d, 0x20, 0x28, 0x78, 0x32, 0x2d, 0x78,
  0x31, 0x29, 0x20, 0x2d, 0x20, 0x32, 0x20, 0x2a, 0x20, 0x73, 0x72, 0x0a,
  0x09, 0x09, 0x68, 0x20, 0x3d, 0x20, 0x28, 0x79, 0x32, 0x2d, 0x79, 0x31,
  0x29, 0x20, 0x2d, 0x20, 0x32, 0x20, 0x2a, 0x20, 0x73, 0x72, 0x0a, 0x0a,
  0x09, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x6c, 0x69, 0x6e, 0x65, 0x28,
  0x78, 0x31, 0x20, 0x2b, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x79, 0x31, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x2c, 0x20, 0x78, 0x32, 0x20, 0x2d, 0x20, 0x73,
  0x72, 0x2c, 0x20, 0x79, 0x31, 0x20, 0x20, 0x20, 0x20, 0x20, 0x29, 0x0a,
  0x09, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x6c, 0x69, 0x6e, 0x65, 0x28,
  0x78, 0x31, 0x20, 0x2b, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x79, 0x32, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x2c, 0x20, 0x78, 0x32, 0x20, 0x2d, 0x20, 0x73,
  0x72, 0x2c, 0x20, 0x79, 0x32, 0x20, 0x20, 0x20, 0x20, 0x20, 0x29, 0x0a,
  0x09, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x6c, 0x69, 0x6e, 0x65, 0x28,
  0x78, 0x31, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2c, 0x20, 0x79, 0x31, 0x20,
  0x2b, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x78, 0x31, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x2c, 0x20, 0x79, 0x32, 0x20, 0x2d, 0x20, 0x73, 0x72, 0x29, 0x0a,
  0x09, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x6c, 0x69, 0x6e, 0x65, 0x28,
  0x78, 0x32, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2c, 0x20, 0x79, 0x31, 0x20,
  0x2b, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x78, 0x32, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x2c, 0x20, 0x79, 0x32, 0x20, 0x2d, 0x20, 0x73, 0x72, 0x29, 0x0a,
  0x0a, 0x09, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x61, 0x72, 0x63, 0x28,
  0x78, 0x32, 0x20, 0x2d, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x79, 0x32, 0x20,
  0x2d, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x70, 0x69,
  0x20, 0x2a, 0x20, 0x30, 0x2e, 0x30, 0x2c, 0x20, 0x70, 0x69, 0x20, 0x2a,
  0x20, 0x30, 0x2e, 0x35, 0x29, 0x0a, 0x09, 0x09, 0x68, 0x65, 0x61, 0x70,
  0x3a, 0x61, 0x72, 0x63, 0x28, 0x78, 0x31, 0x20, 0x2b, 0x20, 0x73, 0x72,
  0x2c, 0x20, 0x79, 0x32, 0x20, 0x2d, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x73,
  0x72, 0x2c, 0x20, 0x70, 0x69, 0x20, 0x2a, 0x20, 0x30, 0x2e, 0x35, 0x2c,
  0x20, 0x70, 0x69, 0x20, 0x2a, 0x20, 0x31, 0x2e, 0x30, 0x29, 0x0a, 0x09,
  0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x61, 0x72, 0x63, 0x28, 0x78, 0x31,
  0x20, 0x2b, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x79, 0x31, 0x20, 0x2b, 0x20,
  0x73, 0x72, 0x2c, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x70, 0x69, 0x20, 0x2a,
  0x20, 0x31, 0x2e, 0x30, 0x2c, 0x20, 0x70, 0x69, 0x20, 0x2a, 0x20, 0x31,
  0x2e, 0x35, 0x29, 0x0a, 0x09, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x61,
  0x72, 0x63, 0x28, 0x78, 0x32, 0x20, 0x2d, 0x20, 0x73, 0x72, 0x2c, 0x20,
  0x79, 0x31, 0x20, 0x2b, 0x20, 0x73, 0x72, 0x2c, 0x20, 0x73, 0x72, 0x2c,
  0x20, 0x70, 0x69, 0x20, 0x2a, 0x20, 0x31, 0x2e, 0x35, 0x2c, 0x20, 0x70,
  0x69, 0x20, 0x2a, 0x20, 0x32, 0x2e, 0x30, 0x29, 0x0a, 0x09, 0x65, 0x6e,
  0x64, 0x0a, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x66, 0x75,
  0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x63, 0x69, 0x72, 0x63, 0x6c,
  0x65, 0x28, 0x68, 0x65, 0x61, 0x70, 0x2c, 0x20, 0x78, 0x2c, 0x20, 0x79,
  0x2c, 0x20, 0x72, 0x29, 0x0a, 0x09, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a,
  0x61, 0x72, 0x63, 0x28, 0x78, 0x2c, 0x20, 0x79, 0x2c, 0x20, 0x72, 0x2c,
  0x20, 0x30, 0x2c, 0x20, 0x70, 0x69, 0x20, 0x2a, 0x20, 0x32, 0x2e, 0x30,
  0x29, 0x0a, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x0a, 0x09, 0x6c, 0x6f, 0x63,
  0x61, 0x6c, 0x20, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20,
  0x61, 0x72, 0x63, 0x28, 0x68, 0x65, 0x61, 0x70, 0x2c, 0x20, 0x2e, 0x2e,
  0x2e, 0x29, 0x0a, 0x09, 0x09, 0x73, 0x65, 0x67, 0x6d, 0x65, 0x6e, 0x74,
  0x5f, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x61, 0x72, 0x63, 0x28, 0x2e, 0x2e,
  0x2e, 0x29, 0x0a, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x0a, 0x09, 0x6c, 0x6f,
  0x63, 0x61, 0x6c, 0x20, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e,
  0x20, 0x6c, 0x69, 0x6e, 0x65, 0x28, 0x68, 0x65, 0x61, 0x70, 0x2c, 0x20,
  0x2e, 0x2e, 0x2e, 0x29, 0x0a, 0x09, 0x09, 0x73, 0x65, 0x67, 0x6d, 0x65,
  0x6e, 0x74, 0x5f, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x6c, 0x69, 0x6e, 0x65,
  0x28, 0x2e, 0x2e, 0x2e, 0x29, 0x0a, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x0a,
  0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x68, 0x65, 0x61, 0x70, 0x20,
  0x3d, 0x20, 0x7b, 0x0a, 0x09, 0x09, 0x74, 0x75, 0x72, 0x74, 0x6c, 0x65,
  0x20, 0x3d, 0x20, 0x74, 0x75, 0x72, 0x74, 0x6c, 0x65, 0x2c, 0x0a, 0x09,
  0x09, 0x62, 0x6f, 0x78, 0x20, 0x3d, 0x20, 0x62, 0x6f, 0x78, 0x2c, 0x0a,
  0x09, 0x09, 0x63, 0x69, 0x72, 0x63, 0x6c, 0x65, 0x20, 0x3d, 0x20, 0x63,
  0x69, 0x72, 0x63, 0x6c, 0x65, 0x2c, 0x0a, 0x09, 0x09, 0x61, 0x72, 0x63,
  0x20, 0x3d, 0x20, 0x61, 0x72, 0x63, 0x2c, 0x0a, 0x09, 0x09, 0x6c, 0x69,
  0x6e, 0x65, 0x20, 0x3d, 0x20, 0x6c, 0x69, 0x6e, 0x65, 0x2c, 0x0a, 0x09,
  0x09, 0x73, 0x65, 0x67, 0x6d, 0x65, 0x6e, 0x74, 0x5f, 0x68, 0x65, 0x61,
  0x70, 0x20, 0x3d, 0x20, 0x73, 0x65, 0x67, 0x6d, 0x65, 0x6e, 0x74, 0x5f,
  0x68, 0x65, 0x61, 0x70, 0x0a, 0x09, 0x7d, 0x0a, 0x0a, 0x09, 0x72, 0x65,
  0x74, 0x75, 0x72, 0x6e, 0x20, 0x68, 0x65, 0x61, 0x70, 0x0a, 0x65, 0x6e,
  0x64, 0x0a
};

unsigned char levels_tofu_lua[] = {
  0x2d, 0x2d, 0x20, 0x45, 0x61, 0x74, 0x20, 0x66, 0x6f, 0x6f, 0x64, 0x20,
  0x69, 0x6e, 0x73, 0x69, 0x64, 0x65, 0x20, 0x61, 0x20, 0x62, 0x6c, 0x6f,
  0x63, 0x6b, 0x20, 0x6f, 0x66, 0x20, 0x74, 0x6f, 0x66, 0x75, 0x0a, 0x0a,
  0x72, 0x65, 0x71, 0x75, 0x69, 0x72, 0x65, 0x20, 0x22, 0x75, 0x74, 0x69,
  0x6c, 0x22, 0x0a, 0x0a, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e,
  0x20, 0x62, 0x65, 0x6e, 0x74, 0x5f, 0x6c, 0x69, 0x6e, 0x65, 0x28, 0x68,
  0x65, 0x61, 0x70, 0x2c, 0x20, 0x41, 0x2c, 0x20, 0x42, 0x2c, 0x20, 0x63,
  0x6f, 0x72, 0x6e, 0x65, 0x72, 0x5f, 0x72, 0x61, 0x64, 0x69, 0x75, 0x73,
  0x2c, 0x20, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x29, 0x0a, 0x09, 0x6c, 0x6f,
  0x63, 0x61, 0x6c, 0x20, 0x41, 0x42, 0x2c, 0x20, 0x6c, 0x72, 0x2c, 0x20,
  0x65, 0x2c, 0x20, 0x66, 0x2c, 0x20, 0x61, 0x6e, 0x67, 0x2c, 0x20, 0x43,
  0x2c, 0x20, 0x62, 0x61, 0x73, 0x65, 0x5f, 0x61, 0x6e, 0x67, 0x6c, 0x65,
  0x2c, 0x20, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x5f, 0x72, 0x61, 0x64, 0x69,
  0x75, 0x73, 0x0a, 0x0a, 0x09, 0x41, 0x42, 0x20, 0x3d, 0x20, 0x42, 0x20,
  0x2b, 0x20, 0x2d, 0x31, 0x20, 0x2a, 0x20, 0x41, 0x0a, 0x0a, 0x09, 0x6c,
  0x72, 0x20, 0x3d, 0x20, 0x41, 0x42, 0x3a, 0x6c, 0x65, 0x6e, 0x67, 0x74,
  0x68, 0x28, 0x29, 0x20, 0x2f, 0x20, 0x28, 0x32, 0x20, 0x2a, 0x20, 0x6d,
  0x61, 0x74, 0x68, 0x2e, 0x73, 0x69, 0x6e, 0x28, 0x70, 0x69, 0x6e, 0x63,
  0x68, 0x29, 0x29, 0x0a, 0x09, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x5f, 0x72,
  0x61, 0x64, 0x69, 0x75, 0x73, 0x20, 0x3d, 0x20, 0x6c, 0x72, 0x20, 0x2d,
  0x20, 0x63, 0x6f, 0x72, 0x6e, 0x65, 0x72, 0x5f, 0x72, 0x61, 0x64, 0x69,
  0x75, 0x73, 0x0a, 0x0a, 0x09, 0x66, 0x20, 0x3d, 0x20, 0x41, 0x42, 0x20,
  0x2a, 0x20, 0x28, 0x31, 0x20, 0x2f, 0x20, 0x41, 0x42, 0x3a, 0x6c, 0x65,
  0x6e, 0x67, 0x74, 0x68, 0x28, 0x29, 0x29, 0x0a, 0x09, 0x65, 0x20, 0x3d,
  0x20, 0x76, 0x65, 0x63, 0x28, 0x66, 0x3a, 0x79, 0x28, 0x29, 0x2c, 0x20,
  0x2d, 0x66, 0x3a, 0x78, 0x28, 0x29, 0x29, 0x0a, 0x09, 0x61, 0x6e, 0x67,
  0x20, 0x3d, 0x20, 0x6d, 0x61, 0x74, 0x68, 0x2e, 0x63, 0x6f, 0x73, 0x28,
  0x70, 0x69, 0x6e, 0x63, 0x68, 0x29, 0x20, 0x2a, 0x20, 0x65, 0x20, 0x2b,
  0x20, 0x6d, 0x61, 0x74, 0x68, 0x2e, 0x73, 0x69, 0x6e, 0x28, 0x70, 0x69,
  0x6e, 0x63, 0x68, 0x29, 0x20, 0x2a, 0x20, 0x66, 0x0a, 0x09, 0x43, 0x20,
  0x3d, 0x20, 0x41, 0x20, 0x2b, 0x20, 0x6c, 0x72, 0x20, 0x2a, 0x20, 0x61,
  0x6e, 0x67, 0x0a, 0x0a, 0x09, 0x62, 0x61, 0x73, 0x65, 0x5f, 0x61, 0x6e,
  0x67, 0x6c, 0x65, 0x20, 0x3d, 0x20, 0x6d, 0x61, 0x74, 0x68, 0x2e, 0x61,
  0x74, 0x61, 0x6e, 0x32, 0x28, 0x2d, 0x65, 0x3a, 0x79, 0x28, 0x29, 0x2c,
  0x20, 0x2d, 0x65, 0x3a, 0x78, 0x28, 0x29, 0x29, 0x0a, 0x0a, 0x09, 0x69,
  0x66, 0x20, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x5f, 0x72, 0x61, 0x64, 0x69,
  0x75, 0x73, 0x20, 0x3c, 0x20, 0x30, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x0a,
  0x09, 0x09, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x5f, 0x72, 0x61, 0x64, 0x69,
  0x75, 0x73, 0x20, 0x3d, 0x20, 0x2d, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x5f,
  0x72, 0x61, 0x64, 0x69, 0x75, 0x73, 0x0a, 0x09, 0x09, 0x62, 0x61, 0x73,
  0x65, 0x5f, 0x61, 0x6e, 0x67, 0x6c, 0x65, 0x20, 0x3d, 0x20, 0x6d, 0x61,
  0x74, 0x68, 0x2e, 0x70, 0x69, 0x20, 0x2b, 0x20, 0x62, 0x61, 0x73, 0x65,
  0x5f, 0x61, 0x6e, 0x67, 0x6c, 0x65, 0x0a, 0x09, 0x65, 0x6e, 0x64, 0x0a,
  0x0a, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x61, 0x72, 0x63, 0x28, 0x0a,
  0x09, 0x09, 0x43, 0x2c, 0x20, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x5f, 0x72,
  0x61, 0x64, 0x69, 0x75, 0x73, 0x2c, 0x0a, 0x09, 0x09, 0x62, 0x61, 0x73,
  0x65, 0x5f, 0x61, 0x6e, 0x67, 0x6c, 0x65, 0x20, 0x2b, 0x20, 0x70, 0x69,
  0x6e, 0x63, 0x68, 0x2c, 0x20, 0x62, 0x61, 0x73, 0x65, 0x5f, 0x61, 0x6e,
  0x67, 0x6c, 0x65, 0x20, 0x2d, 0x20, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x0a,
  0x09, 0x29, 0x0a, 0x65, 0x6e, 0x64, 0x0a, 0x0a, 0x66, 0x75, 0x6e, 0x63,
  0x74, 0x69, 0x6f, 0x6e, 0x20, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x5f,
  0x62, 0x6f, 0x61, 0x72, 0x64, 0x28, 0x29, 0x0a, 0x09, 0x6c, 0x6f, 0x63,
  0x61, 0x6c, 0x20, 0x68, 0x65, 0x61, 0x70, 0x20, 0x3d, 0x20, 0x75, 0x74,
  0x69, 0x6c, 0x5f, 0x68, 0x65, 0x61, 0x70, 0x28, 0x29, 0x0a, 0x09, 0x6c,
  0x6f, 0x63, 0x61, 0x6c, 0x20, 0x70, 0x69, 0x20, 0x3d, 0x20, 0x6d, 0x61,
  0x74, 0x68, 0x2e, 0x70, 0x69, 0x0a, 0x0a, 0x09, 0x68, 0x65, 0x61, 0x70,
  0x2e, 0x62, 0x65, 0x6e, 0x74, 0x5f, 0x6c, 0x69, 0x6e, 0x65, 0x20, 0x3d,
  0x20, 0x62, 0x65, 0x6e, 0x74, 0x5f, 0x6c, 0x69, 0x6e, 0x65, 0x0a, 0x0a,
  0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x63, 0x6f, 0x72, 0x6e, 0x65,
  0x72, 0x5f, 0x72, 0x61, 0x64, 0x69, 0x75, 0x73, 0x20, 0x3d, 0x20, 0x31,
  0x30, 0x0a, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x70, 0x69,
  0x6e, 0x63, 0x68, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x33, 0x0a, 0x0a, 0x09,
  0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x68, 0x65, 0x69, 0x67, 0x68, 0x74,
  0x20, 0x3d, 0x20, 0x39, 0x30, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c,
  0x20, 0x77, 0x69, 0x64, 0x74, 0x68, 0x20, 0x3d, 0x20, 0x31, 0x36, 0x30,
  0x0a, 0x0a, 0x09, 0x2d, 0x2d, 0x20, 0x49, 0x6e, 0x6e, 0x65, 0x72, 0x20,
  0x64, 0x69, 0x6d, 0x65, 0x6e, 0x73, 0x69, 0x6f, 0x6e, 0x73, 0x3a, 0x0a,
  0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x6c, 0x65, 0x66, 0x74, 0x20,
  0x3d, 0x20, 0x30, 0x20, 0x2d, 0x20, 0x77, 0x69, 0x64, 0x74, 0x68, 0x2f,
  0x32, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x72, 0x69, 0x67,
  0x68, 0x74, 0x20, 0x3d, 0x20, 0x6c, 0x65, 0x66, 0x74, 0x20, 0x2b, 0x20,
  0x77, 0x69, 0x64, 0x74, 0x68, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c,
  0x20, 0x62, 0x6f, 0x74, 0x74, 0x6f, 0x6d, 0x20, 0x3d, 0x20, 0x30, 0x20,
  0x2d, 0x20, 0x68, 0x65, 0x69, 0x67, 0x68, 0x74, 0x2f, 0x32, 0x20, 0x2d,
  0x20, 0x31, 0x30, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x74,
  0x6f, 0x70, 0x20, 0x3d, 0x20, 0x62, 0x6f, 0x74, 0x74, 0x6f, 0x6d, 0x20,
  0x2b, 0x20, 0x68, 0x65, 0x69, 0x67, 0x68, 0x74, 0x0a, 0x0a, 0x09, 0x6c,
  0x6f, 0x63, 0x61, 0x6c, 0x20, 0x41, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63,
  0x28, 0x6c, 0x65, 0x66, 0x74, 0x2c, 0x20, 0x62, 0x6f, 0x74, 0x74, 0x6f,
  0x6d, 0x29, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x42, 0x20,
  0x3d, 0x20, 0x76, 0x65, 0x63, 0x28, 0x72, 0x69, 0x67, 0x68, 0x74, 0x2c,
  0x20, 0x62, 0x6f, 0x74, 0x74, 0x6f, 0x6d, 0x29, 0x0a, 0x09, 0x6c, 0x6f,
  0x63, 0x61, 0x6c, 0x20, 0x43, 0x20, 0x3d, 0x20, 0x76, 0x65, 0x63, 0x28,
  0x72, 0x69, 0x67, 0x68, 0x74, 0x2c, 0x20, 0x74, 0x6f, 0x70, 0x29, 0x0a,
  0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x44, 0x20, 0x3d, 0x20, 0x76,
  0x65, 0x63, 0x28, 0x6c, 0x65, 0x66, 0x74, 0x2c, 0x20, 0x74, 0x6f, 0x70,
  0x29, 0x0a, 0x0a, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x61, 0x72, 0x63,
  0x28, 0x41, 0x2c, 0x20, 0x63, 0x6f, 0x72, 0x6e, 0x65, 0x72, 0x5f, 0x72,
  0x61, 0x64, 0x69, 0x75, 0x73, 0x2c, 0x20, 0x32, 0x2f, 0x32, 0x20, 0x2a,
  0x20, 0x70, 0x69, 0x20, 0x2b, 0x20, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x2c,
  0x20, 0x33, 0x2f, 0x32, 0x20, 0x2a, 0x20, 0x70, 0x69, 0x20, 0x2b, 0x20,
  0x70, 0x69, 0x6e, 0x63, 0x68, 0x29, 0x0a, 0x09, 0x68, 0x65, 0x61, 0x70,
  0x3a, 0x62, 0x65, 0x6e, 0x74, 0x5f, 0x6c, 0x69, 0x6e, 0x65, 0x28, 0x41,
  0x2c, 0x20, 0x42, 0x2c, 0x20, 0x63, 0x6f, 0x72, 0x6e, 0x65, 0x72, 0x5f,
  0x72, 0x61, 0x64, 0x69, 0x75, 0x73, 0x2c, 0x20, 0x70, 0x69, 0x6e, 0x63,
  0x68, 0x29, 0x0a, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x61, 0x72, 0x63,
  0x28, 0x42, 0x2c, 0x20, 0x63, 0x6f, 0x72, 0x6e, 0x65, 0x72, 0x5f, 0x72,
  0x61, 0x64, 0x69, 0x75, 0x73, 0x2c, 0x20, 0x33, 0x2f, 0x32, 0x20, 0x2a,
  0x20, 0x70, 0x69, 0x20, 0x2d, 0x20, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x2c,
  0x20, 0x34, 0x2f, 0x32, 0x20, 0x2a, 0x20, 0x70, 0x69, 0x20, 0x2d, 0x20,
  0x70, 0x69, 0x6e, 0x63, 0x68, 0x29, 0x0a, 0x09, 0x68, 0x65, 0x61, 0x70,
  0x3a, 0x62, 0x65, 0x6e, 0x74, 0x5f, 0x6c, 0x69, 0x6e, 0x65, 0x28, 0x42,
  0x2c, 0x20, 0x43, 0x2c, 0x20, 0x63, 0x6f, 0x72, 0x6e, 0x65, 0x72, 0x5f,
  0x72, 0x61, 0x64, 0x69, 0x75, 0x73, 0x2c, 0x20, 0x2d, 0x70, 0x69, 0x6e,
  0x63, 0x68, 0x29, 0x0a, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x61, 0x72,
  0x63, 0x28, 0x43, 0x2c, 0x20, 0x63, 0x6f, 0x72, 0x6e, 0x65, 0x72, 0x5f,
  0x72, 0x61, 0x64, 0x69, 0x75, 0x73, 0x2c, 0x20, 0x30, 0x2f, 0x32, 0x20,
  0x2a, 0x20, 0x70, 0x69, 0x20, 0x2b, 0x20, 0x70, 0x69, 0x6e, 0x63, 0x68,
  0x2c, 0x20, 0x31, 0x2f, 0x32, 0x20, 0x2a, 0x20, 0x70, 0x69, 0x20, 0x2b,
  0x20, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x29, 0x0a, 0x09, 0x68, 0x65, 0x61,
  0x70, 0x3a, 0x62, 0x65, 0x6e, 0x74, 0x5f, 0x6c, 0x69, 0x6e, 0x65, 0x28,
  0x43, 0x2c, 0x20, 0x44, 0x2c, 0x20, 0x63, 0x6f, 0x72, 0x6e, 0x65, 0x72,
  0x5f, 0x72, 0x61, 0x64, 0x69, 0x75, 0x73, 0x2c, 0x20, 0x70, 0x69, 0x6e,
  0x63, 0x68, 0x29, 0x0a, 0x09, 0x68, 0x65, 0x61, 0x70, 0x3a, 0x61, 0x72,
  0x63, 0x28, 0x44, 0x2c, 0x20, 0x63, 0x6f, 0x72, 0x6e, 0x65, 0x72, 0x5f,
  0x72, 0x61, 0x64, 0x69, 0x75, 0x73, 0x2c, 0x20, 0x31, 0x2f, 0x32, 0x20,
  0x2a, 0x20, 0x70, 0x69, 0x20, 0x2d, 0x20, 0x70, 0x69, 0x6e, 0x63, 0x68,
  0x2c, 0x20, 0x32, 0x2f, 0x32, 0x20, 0x2a, 0x20, 0x70, 0x69, 0x20, 0x2d,
  0x20, 0x70, 0x69, 0x6e, 0x63, 0x68, 0x29, 0x0a, 0x09, 0x68, 0x65, 0x61,
  0x70, 0x3a, 0x62, 0x65, 0x6e, 0x74, 0x5f, 0x6c, 0x69, 0x6e, 0x65, 0x28,
  0x44, 0x2c, 0x20, 0x41, 0x2c, 0x20, 0x63, 0x6f, 0x72, 0x6e, 0x65, 0x72,
  0x5f, 0x72, 0x61, 0x64, 0x69, 0x75, 0x73, 0x2c, 0x20, 0x2d, 0x70, 0x69,
  0x6e, 0x63, 0x68, 0x29, 0x0a, 0x0a, 0x09, 0x72, 0x65, 0x74, 0x75, 0x72,
  0x6e, 0x20, 0x68, 0x65, 0x61, 0x70, 0x0a, 0x65, 0x6e, 0x64, 0x0a
};

unsigned char levels_pacman_lua[] = {
  0x2d, 0x2d, 0x20, 0x50, 0x61, 0x63, 0x6d, 0x61, 0x6e, 0x0a, 0x0a, 0x72,
  0x65, 0x71, 0x75, 0x69, 0x72, 0x65, 0x20, 0x22, 0x75, 0x74, 0x69, 0x6c,
  0x22, 0x0a, 0x0a, 0x53, 0x5a, 0x20, 0x3d, 0x20, 0x31, 0x30, 0x0a, 0x0a,
  0x50, 0x41, 0x43, 0x4d, 0x41, 0x4e, 0x5f, 0x42, 0x4f, 0x41, 0x52, 0x44,
  0x20, 0x3d, 0x20, 0x7b, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x58, 0x58, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x58, 0x58, 0x22,
  0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x2e,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x22, 0x2c,
  0x0a, 0x09, 0x22, 0x58, 0x58, 0x2a, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x2a, 0x58, 0x58, 0x22, 0x2c, 0x0a,
  0x09, 0x22, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x2e, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09,
  0x22, 0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22,
  0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58,
  0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58,
  0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58,
  0x58, 0x58, 0x2e, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e,
  0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e, 0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20, 0x58,
  0x58, 0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20, 0x58, 0x58,
  0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x2e, 0x58, 0x58, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x2e, 0x58, 0x58, 0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x20, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x2e, 0x58, 0x58, 0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x20, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x22,
  0x2c, 0x0a, 0x09, 0x22, 0x78, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x2e,
  0x20, 0x20, 0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20,
  0x20, 0x20, 0x2e, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x78, 0x22, 0x2c,
  0x0a, 0x09, 0x22, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58,
  0x58, 0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20, 0x58,
  0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x22, 0x2c, 0x0a,
  0x09, 0x22, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58,
  0x20, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20, 0x58, 0x58,
  0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09,
  0x22, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x58, 0x58, 0x2e,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x20, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20, 0x58, 0x58, 0x2e, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x20, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x20, 0x58, 0x58, 0x2e, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x2e,
  0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58,
  0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58,
  0x2e, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x2e, 0x58,
  0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58,
  0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x2e,
  0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x2a, 0x2e, 0x2e,
  0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x20, 0x20, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x58, 0x58, 0x2e, 0x2e, 0x2a, 0x58,
  0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58,
  0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58,
  0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58,
  0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x22,
  0x2c, 0x0a, 0x09, 0x22, 0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e, 0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e,
  0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x58, 0x58, 0x22, 0x2c,
  0x0a, 0x09, 0x22, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x22, 0x2c, 0x0a,
  0x09, 0x22, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x2e, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09,
  0x22, 0x58, 0x58, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e,
  0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x22, 0x2c, 0x0a, 0x09, 0x22, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58,
  0x58, 0x58, 0x58, 0x58, 0x58, 0x22, 0x0a, 0x7d, 0x0a, 0x0a, 0x66, 0x75,
  0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70,
  0x65, 0x6e, 0x28, 0x78, 0x2c, 0x20, 0x79, 0x29, 0x0a, 0x09, 0x69, 0x66,
  0x20, 0x79, 0x20, 0x3c, 0x3d, 0x20, 0x30, 0x20, 0x6f, 0x72, 0x20, 0x79,
  0x20, 0x3e, 0x20, 0x23, 0x50, 0x41, 0x43, 0x4d, 0x41, 0x4e, 0x5f, 0x42,
  0x4f, 0x41, 0x52, 0x44, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x20, 0x72, 0x65,
  0x74, 0x75, 0x72, 0x6e, 0x20, 0x66, 0x61, 0x6c, 0x73, 0x65, 0x20, 0x65,
  0x6e, 0x64, 0x0a, 0x09, 0x69, 0x66, 0x20, 0x78, 0x20, 0x3c, 0x3d, 0x20,
  0x30, 0x20, 0x6f, 0x72, 0x20, 0x78, 0x20, 0x3e, 0x20, 0x23, 0x50, 0x41,
  0x43, 0x4d, 0x41, 0x4e, 0x5f, 0x42, 0x4f, 0x41, 0x52, 0x44, 0x5b, 0x79,
  0x5d, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72,
  0x6e, 0x20, 0x66, 0x61, 0x6c, 0x73, 0x65, 0x20, 0x65, 0x6e, 0x64, 0x0a,
  0x09, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6e, 0x20, 0x28, 0x50, 0x41, 0x43,
  0x4d, 0x41, 0x4e, 0x5f, 0x42, 0x4f, 0x41, 0x52, 0x44, 0x5b, 0x79, 0x5d,
  0x3a, 0x73, 0x75, 0x62, 0x28, 0x78, 0x2c, 0x20, 0x78, 0x29, 0x20, 0x7e,
  0x3d, 0x20, 0x22, 0x58, 0x22, 0x29, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x28,
  0x50, 0x41, 0x43, 0x4d, 0x41, 0x4e, 0x5f, 0x42, 0x4f, 0x41, 0x52, 0x44,
  0x5b, 0x79, 0x5d, 0x3a, 0x73, 0x75, 0x62, 0x28, 0x78, 0x2c, 0x20, 0x78,
  0x29, 0x20, 0x7e, 0x3d, 0x20, 0x22, 0x78, 0x22, 0x29, 0x0a, 0x65, 0x6e,
  0x64, 0x0a, 0x0a, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20,
  0x63, 0x6f, 0x6e, 0x73, 0x69, 0x64, 0x65, 0x72, 0x5f, 0x73, 0x71, 0x75,
  0x61, 0x72, 0x65, 0x28, 0x68, 0x65, 0x61, 0x70, 0x2c, 0x20, 0x6f, 0x78,
  0x2c, 0x20, 0x6f, 0x79, 0x2c, 0x20, 0x78, 0x2c, 0x20, 0x79, 0x29, 0x0a,
  0x09, 0x69, 0x66, 0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28,
  0x78, 0x2c, 0x20, 0x79, 0x29, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x20, 0x72,
  0x65, 0x74, 0x75, 0x72, 0x6e, 0x20, 0x65, 0x6e, 0x64, 0x0a, 0x0a, 0x09,
  0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x6c, 0x78, 0x2c, 0x20, 0x6c, 0x79,
  0x2c, 0x20, 0x68, 0x78, 0x2c, 0x20, 0x68, 0x79, 0x2c, 0x20, 0x63, 0x78,
  0x2c, 0x20, 0x63, 0x79, 0x0a, 0x0a, 0x09, 0x6c, 0x78, 0x20, 0x3d, 0x20,
  0x6f, 0x78, 0x20, 0x2b, 0x20, 0x28, 0x78, 0x2d, 0x31, 0x29, 0x20, 0x2a,
  0x20, 0x53, 0x5a, 0x0a, 0x09, 0x68, 0x79, 0x20, 0x3d, 0x20, 0x6f, 0x79,
  0x20, 0x2d, 0x20, 0x28, 0x79, 0x2d, 0x31, 0x29, 0x20, 0x2a, 0x20, 0x53,
  0x5a, 0x0a, 0x09, 0x68, 0x78, 0x20, 0x3d, 0x20, 0x6c, 0x78, 0x20, 0x2b,
  0x20, 0x53, 0x5a, 0x0a, 0x09, 0x6c, 0x79, 0x20, 0x3d, 0x20, 0x68, 0x79,
  0x20, 0x2d, 0x20, 0x53, 0x5a, 0x0a, 0x09, 0x63, 0x78, 0x20, 0x3d, 0x20,
  0x28, 0x6c, 0x78, 0x20, 0x2b, 0x20, 0x68, 0x78, 0x29, 0x20, 0x2f, 0x20,
  0x32, 0x0a, 0x09, 0x63, 0x79, 0x20, 0x3d, 0x20, 0x28, 0x6c, 0x79, 0x20,
  0x2b, 0x20, 0x68, 0x79, 0x29, 0x20, 0x2f, 0x20, 0x32, 0x0a, 0x0a, 0x09,
  0x6e, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x73, 0x20, 0x3d, 0x20, 0x30, 0x0a,
  0x09, 0x69, 0x66, 0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28,
  0x78, 0x2d, 0x31, 0x2c, 0x20, 0x79, 0x29, 0x20, 0x74, 0x68, 0x65, 0x6e,
  0x20, 0x6e, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x73, 0x20, 0x3d, 0x20, 0x6e,
  0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x73, 0x20, 0x2b, 0x20, 0x31, 0x20, 0x65,
  0x6e, 0x64, 0x0a, 0x09, 0x69, 0x66, 0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70,
  0x65, 0x6e, 0x28, 0x78, 0x2b, 0x31, 0x2c, 0x20, 0x79, 0x29, 0x20, 0x74,
  0x68, 0x65, 0x6e, 0x20, 0x6e, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x73, 0x20,
  0x3d, 0x20, 0x6e, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x73, 0x20, 0x2b, 0x20,
  0x31, 0x20, 0x65, 0x6e, 0x64, 0x0a, 0x09, 0x69, 0x66, 0x20, 0x69, 0x73,
  0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28, 0x78, 0x2c, 0x20, 0x79, 0x2d, 0x31,
  0x29, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x20, 0x6e, 0x5f, 0x6f, 0x70, 0x65,
  0x6e, 0x73, 0x20, 0x3d, 0x20, 0x6e, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x73,
  0x20, 0x2b, 0x20, 0x31, 0x20, 0x65, 0x6e, 0x64, 0x0a, 0x09, 0x69, 0x66,
  0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28, 0x78, 0x2c, 0x20,
  0x79, 0x2b, 0x31, 0x29, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x20, 0x6e, 0x5f,
  0x6f, 0x70, 0x65, 0x6e, 0x73, 0x20, 0x3d, 0x20, 0x6e, 0x5f, 0x6f, 0x70,
  0x65, 0x6e, 0x73, 0x20, 0x2b, 0x20, 0x31, 0x20, 0x65, 0x6e, 0x64, 0x0a,
  0x0a, 0x09, 0x69, 0x66, 0x20, 0x6e, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x73,
  0x20, 0x3d, 0x3d, 0x20, 0x31, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x0a, 0x09,
  0x09, 0x69, 0x66, 0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28,
  0x78, 0x2d, 0x31, 0x2c, 0x20, 0x79, 0x29, 0x20, 0x6f, 0x72, 0x20, 0x69,
  0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28, 0x78, 0x2b, 0x31, 0x2c, 0x20,
  0x79, 0x29, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x0a, 0x09, 0x09, 0x09, 0x68,
  0x65, 0x61, 0x70, 0x3a, 0x6c, 0x69, 0x6e, 0x65, 0x28, 0x63, 0x78, 0x2c,
  0x20, 0x6c, 0x79, 0x2c, 0x20, 0x63, 0x78, 0x2c, 0x20, 0x68, 0x79, 0x29,
  0x0a, 0x09, 0x09, 0x65, 0x6c, 0x73, 0x65, 0x0a, 0x09, 0x09, 0x09, 0x68,
  0x65, 0x61, 0x70, 0x3a, 0x6c, 0x69, 0x6e, 0x65, 0x28, 0x6c, 0x78, 0x2c,
  0x20, 0x63, 0x79, 0x2c, 0x20, 0x68, 0x78, 0x2c, 0x20, 0x63, 0x79, 0x29,
  0x0a, 0x09, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x09, 0x65, 0x6c, 0x73, 0x65,
  0x69, 0x66, 0x20, 0x6e, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x73, 0x20, 0x3d,
  0x3d, 0x20, 0x32, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x0a, 0x09, 0x09, 0x62,
  0x20, 0x3d, 0x20, 0x30, 0x0a, 0x0a, 0x09, 0x09, 0x61, 0x72, 0x63, 0x5f,
  0x78, 0x20, 0x3d, 0x20, 0x6c, 0x78, 0x0a, 0x09, 0x09, 0x69, 0x66, 0x20,
  0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28, 0x78, 0x2d, 0x31, 0x2c,
  0x20, 0x79, 0x29, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x0a, 0x09, 0x09, 0x09,
  0x61, 0x72, 0x63, 0x5f, 0x78, 0x20, 0x3d, 0x20, 0x68, 0x78, 0x0a, 0x09,
  0x09, 0x09, 0x62, 0x20, 0x3d, 0x20, 0x62, 0x20, 0x2b, 0x20, 0x6d, 0x61,
  0x74, 0x68, 0x2e, 0x70, 0x69, 0x20, 0x2a, 0x20, 0x30, 0x2e, 0x35, 0x0a,
  0x09, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x0a, 0x09, 0x09, 0x61, 0x72, 0x63,
  0x5f, 0x79, 0x20, 0x3d, 0x20, 0x6c, 0x79, 0x0a, 0x09, 0x09, 0x69, 0x66,
  0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28, 0x78, 0x2c, 0x20,
  0x79, 0x2b, 0x31, 0x29, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x0a, 0x09, 0x09,
  0x09, 0x61, 0x72, 0x63, 0x5f, 0x79, 0x20, 0x3d, 0x20, 0x68, 0x79, 0x0a,
  0x09, 0x09, 0x09, 0x62, 0x20, 0x3d, 0x20, 0x6d, 0x61, 0x74, 0x68, 0x2e,
  0x70, 0x69, 0x20, 0x2a, 0x20, 0x33, 0x2f, 0x32, 0x20, 0x2d, 0x20, 0x62,
  0x0a, 0x09, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x0a, 0x09, 0x09, 0x68, 0x65,
  0x61, 0x70, 0x3a, 0x61, 0x72, 0x63, 0x28, 0x61, 0x72, 0x63, 0x5f, 0x78,
  0x2c, 0x20, 0x61, 0x72, 0x63, 0x5f, 0x79, 0x2c, 0x20, 0x53, 0x5a, 0x2f,
  0x32, 0x2c, 0x20, 0x62, 0x2c, 0x20, 0x62, 0x20, 0x2b, 0x20, 0x6d, 0x61,
  0x74, 0x68, 0x2e, 0x70, 0x69, 0x20, 0x2a, 0x20, 0x30, 0x2e, 0x35, 0x29,
  0x0a, 0x09, 0x65, 0x6c, 0x73, 0x65, 0x69, 0x66, 0x20, 0x6e, 0x5f, 0x6f,
  0x70, 0x65, 0x6e, 0x73, 0x20, 0x3d, 0x3d, 0x20, 0x30, 0x20, 0x74, 0x68,
  0x65, 0x6e, 0x0a, 0x09, 0x09, 0x69, 0x66, 0x20, 0x69, 0x73, 0x5f, 0x6f,
  0x70, 0x65, 0x6e, 0x28, 0x78, 0x2d, 0x31, 0x2c, 0x20, 0x79, 0x2d, 0x31,
  0x29, 0x20, 0x6f, 0x72, 0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e,
  0x28, 0x78, 0x2d, 0x31, 0x2c, 0x20, 0x79, 0x2b, 0x31, 0x29, 0x20, 0x6f,
  0x72, 0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28, 0x78, 0x2b,
  0x31, 0x2c, 0x20, 0x79, 0x2d, 0x31, 0x29, 0x20, 0x6f, 0x72, 0x20, 0x69,
  0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28, 0x78, 0x2b, 0x31, 0x2c, 0x20,
  0x79, 0x2b, 0x31, 0x29, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x0a, 0x09, 0x09,
  0x09, 0x61, 0x72, 0x63, 0x5f, 0x78, 0x20, 0x3d, 0x20, 0x6c, 0x78, 0x0a,
  0x09, 0x09, 0x09, 0x64, 0x78, 0x20, 0x3d, 0x20, 0x31, 0x0a, 0x09, 0x09,
  0x09, 0x61, 0x6e, 0x67, 0x20, 0x3d, 0x20, 0x6d, 0x61, 0x74, 0x68, 0x2e,
  0x70, 0x69, 0x20, 0x2a, 0x20, 0x30, 0x2e, 0x35, 0x0a, 0x0a, 0x09, 0x09,
  0x09, 0x69, 0x66, 0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28,
  0x78, 0x2b, 0x31, 0x2c, 0x20, 0x79, 0x2d, 0x31, 0x29, 0x20, 0x6f, 0x72,
  0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e, 0x28, 0x78, 0x2b, 0x31,
  0x2c, 0x20, 0x79, 0x2b, 0x31, 0x29, 0x20, 0x74, 0x68, 0x65, 0x6e, 0x0a,
  0x09, 0x09, 0x09, 0x09, 0x61, 0x72, 0x63, 0x5f, 0x78, 0x20, 0x3d, 0x20,
  0x68, 0x78, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x64, 0x78, 0x20, 0x3d, 0x20,
  0x2d, 0x31, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x61, 0x6e, 0x67, 0x20, 0x3d,
  0x20, 0x2d, 0x61, 0x6e, 0x67, 0x0a, 0x09, 0x09, 0x09, 0x65, 0x6e, 0x64,
  0x0a, 0x0a, 0x09, 0x09, 0x09, 0x69, 0x66, 0x20, 0x69, 0x73, 0x5f, 0x6f,
  0x70, 0x65, 0x6e, 0x28, 0x78, 0x2d, 0x31, 0x2c, 0x20, 0x79, 0x2b, 0x31,
  0x29, 0x20, 0x6f, 0x72, 0x20, 0x69, 0x73, 0x5f, 0x6f, 0x70, 0x65, 0x6e,
  0x28, 0x78, 0x2b, 0x31, 0x2c, 0x20, 0x79, 0x2b, 0x31, 0x29, 0x20, 0x74,
  0x68, 0x65, 0x6e, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x61, 0x6e, 0x67, 0x20,
  0x3d, 0x20, 0x2d, 0x61, 0x6e, 0x67, 0x0a, 0x09, 0x09, 0x09, 0x65, 0x6e,
  0x64, 0x0a, 0x0a, 0x09, 0x09, 0x09, 0x74, 0x20, 0x3d, 0x20, 0x68, 0x65,
  0x61, 0x70, 0x3a, 0x74, 0x75, 0x72, 0x74, 0x6c, 0x65, 0x28, 0x61, 0x72,
  0x63, 0x5f, 0x78, 0x2c, 0x20, 0x63, 0x79, 0x2c, 0x20, 0x64, 0x78, 0x2c,
  0x20, 0x30, 0x29, 0x0a, 0x09, 0x09, 0x09, 0x74, 0x2e, 0x66, 0x6f, 0x72,
  0x77, 0x61, 0x72, 0x64, 0x28, 0x53, 0x5a, 0x2a, 0x30, 0x2e, 0x31, 0x35,
  0x29, 0x0a, 0x09, 0x09, 0x09, 0x74, 0x2e, 0x6c, 0x65, 0x66, 0x74, 0x28,
  0x53, 0x5a, 0x2a, 0x30, 0x2e, 0x33, 0x35, 0x2c, 0x20, 0x61, 0x6e, 0x67,
  0x29, 0x0a, 0x09, 0x09, 0x09, 0x74, 0x2e, 0x66, 0x6f, 0x72, 0x77, 0x61,
  0x72, 0x64, 0x28, 0x53, 0x5a, 0x2a, 0x30, 0x2e, 0x31, 0x35, 0x29, 0x0a,
  0x09, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x65,
  0x6e, 0x64, 0x0a, 0x0a, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e,
  0x20, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x5f, 0x62, 0x6f, 0x61, 0x72,
  0x64, 0x28, 0x29, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x68,
  0x20, 0x3d, 0x20, 0x23, 0x20, 0x50, 0x41, 0x43, 0x4d, 0x41, 0x4e, 0x5f,
  0x42, 0x4f, 0x41, 0x52, 0x44, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c,
  0x20, 0x77, 0x20, 0x3d, 0x20, 0x23, 0x20, 0x50, 0x41, 0x43, 0x4d, 0x41,
  0x4e, 0x5f, 0x42, 0x4f, 0x41, 0x52, 0x44, 0x5b, 0x31, 0x5d, 0x0a, 0x0a,
  0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x6f, 0x78, 0x20, 0x3d, 0x20,
  0x2d, 0x30, 0x2e, 0x35, 0x20, 0x2a, 0x20, 0x53, 0x5a, 0x20, 0x2a, 0x20,
  0x77, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20, 0x6f, 0x79, 0x20,
  0x3d, 0x20, 0x20, 0x30, 0x2e, 0x35, 0x20, 0x2a, 0x20, 0x53, 0x5a, 0x20,
  0x2a, 0x20, 0x68, 0x0a, 0x0a, 0x09, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x20,
  0x68, 0x65, 0x61, 0x70, 0x20, 0x3d, 0x20, 0x75, 0x74, 0x69, 0x6c, 0x5f,
  0x68, 0x65, 0x61, 0x70, 0x28, 0x29, 0x0a, 0x0a, 0x09, 0x6c, 0x6f, 0x63,
  0x61, 0x6c, 0x20, 0x78, 0x2c, 0x20, 0x79, 0x0a, 0x09, 0x66, 0x6f, 0x72,
  0x20, 0x79, 0x20, 0x3d, 0x20, 0x31, 0x2c, 0x20, 0x68, 0x20, 0x64, 0x6f,
  0x0a, 0x09, 0x09, 0x66, 0x6f, 0x72, 0x20, 0x78, 0x20, 0x3d, 0x20, 0x31,
  0x2c, 0x20, 0x77, 0x20, 0x64, 0x6f, 0x0a, 0x09, 0x09, 0x09, 0x63, 0x6f,
  0x6e, 0x73, 0x69, 0x64, 0x65, 0x72, 0x5f, 0x73, 0x71, 0x75, 0x61, 0x72,
  0x65, 0x28, 0x68, 0x65, 0x61, 0x70, 0x2c, 0x20, 0x6f, 0x78, 0x2c, 0x20,
  0x6f, 0x79, 0x2c, 0x20, 0x78, 0x2c, 0x20, 0x79, 0x29, 0x0a, 0x09, 0x09,
  0x65, 0x6e, 0x64, 0x0a, 0x09, 0x65, 0x6e, 0x64, 0x0a, 0x0a, 0x09, 0x72,
  0x65, 0x74, 0x75, 0x72, 0x6e, 0x20, 0x68, 0x65, 0x61, 0x70, 0x0a, 0x65,
  0x6e, 0x64, 0x0a, 0x0a, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e,
  0x20, 0x67, 0x65, 0x74, 0x5f, 0x73, 0x74, 0x61, 0x72, 0x74, 0x69, 0x6e,
  0x67, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x28, 0x29,
  0x0a, 0x09, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6e, 0x20, 0x76, 0x65, 0x63,
  0x28, 0x30, 0x2c, 0x20, 0x2d, 0x53, 0x5a, 0x20, 0x2a, 0x20, 0x38, 0x2e,
  0x35, 0x29, 0x0a, 0x65, 0x6e, 0x64, 0x0a
};

std::map<std::string, std::string> modules{
	{ "util", { std::begin(levels_util_lua), std::end(levels_util_lua) } },
	{ "tofu", { std::begin(levels_tofu_lua), std::end(levels_tofu_lua) } },
	{ "pacman", { std::begin(levels_pacman_lua), std::end(levels_pacman_lua) } },
};
