#include "fileutil.hpp"
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>

void read_entire_file(std::vector<char>& target, const std::string& filename) {
	std::ifstream in(filename.c_str(), std::ios::binary);
	if (!in.is_open()) throw std::runtime_error("Unable to open file: " + filename);

	in.seekg(0, std::ios::end);
	target.resize(in.tellg());
	in.seekg(0, std::ios::beg);

	in.read(&target[0], target.size());
	in.close();
}

void read_entire_stream(std::vector<char>& target, std::istream& in) {
	const size_t buf_size = 4096;

	while (in.good()) {
		auto base = target.size();
		target.resize(base + buf_size);
		in.read(target.data() + base, buf_size);
		target.resize(base + in.gcount());
	}
}
