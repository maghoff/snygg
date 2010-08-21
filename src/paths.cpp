#include <cstdlib>
#include <fstream>
#include "config.hpp"
#include "paths.hpp"

using boost::filesystem::path;

namespace {

std::string g_argv_zero;

std::string get_from_proc() {
	char buffer[FILENAME_MAX + 1];
	int len = readlink("/proc/self/exe", buffer, FILENAME_MAX);
	if (len > 0) return std::string(buffer, buffer+len);
	return std::string();
}

}

namespace paths {

void set_argv_zero(const char* argv_zero) {
	g_argv_zero = argv_zero;
}

path executed_file() {
	std::string p;

	if (p.empty()) p = get_from_proc();
	if (p.empty()) p = g_argv_zero;
	if (p.empty()) p = "snygg"; // Fail. Give a stupid default

	return path(p);
}

path executable_path() {
	return executed_file().branch_path(); //< remove "snygg"
}

bool is_installed() {
	path p(executable_path());

	if (p.filename() == "src") return false;

	return true;
}

path guess_prefix() {
	path configured_prefix(config::install_prefix);

	path p(executable_path());
	if (p.filename() == "games" || p.filename() == "bin") return p.branch_path();
	else return configured_prefix;
}

path guess_project_dir() {
	path p(executable_path());
	if (p.filename() == "src") p = p.branch_path();
	if (p.filename() == "debug" || p.filename() == "release") p = p.branch_path();
	return p;
}

path data() {
	if (is_installed()) return guess_prefix() / "share/games/snygg";
	else return guess_project_dir();
}

path levels() {
	return data() / "levels";
}

path skins() {
	return data() / "skins";
}

path open_absolute_or_in_path(
	std::ifstream& in,
	const path& requested_path,
	const path& search_path
) {
	if (!requested_path.is_complete()) {
		path try_path = search_path / requested_path;
		in.open(try_path.file_string().c_str());
		if (in.is_open()) return try_path;
	}

	in.open(requested_path.file_string().c_str());
	if (in.is_open()) return requested_path;

	return path();
}

path find_absolute_or_in_path(
	const path& requested_path,
	const path& search_path
) {
	std::ifstream in;
	return open_absolute_or_in_path(in, requested_path, search_path);
}

}
