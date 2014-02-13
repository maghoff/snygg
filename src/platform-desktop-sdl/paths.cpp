#ifdef __MACH__
#include <CoreFoundation/CFBundle.h>
#endif

#include <iostream>

#include <cstdlib>
#include <fstream>
#include <lean_windows.h>
#include "../config.hpp"
#include "paths.hpp"

using boost::filesystem::path;

namespace {

std::string g_argv_zero;

#ifdef __linux__
path get_executed_file_from_proc() {
	char buffer[FILENAME_MAX + 1];
	int len = readlink("/proc/self/exe", buffer, FILENAME_MAX);
	if (len > 0) return path(buffer, buffer+len);
	return path();
}
#endif

#if defined(_WIN32)
path get_executed_file_from_win32_api() {
	const size_t sz = MAX_PATH + 1;
    std::vector<wchar_t> exepath(sz);
    DWORD actual_len = GetModuleFileNameW(NULL, exepath.data(), sz);
    if (actual_len == 0) return path();

    path p(exepath.data(), exepath.data() + actual_len);
	return p;
}
#endif

#ifdef __MACH__
path get_bundle_resources_directory() {
	path ret;
	char buf[FILENAME_MAX+1];
	CFBundleRef main_bundle;
	CFURLRef resources_url;
	CFStringRef resources_str;
	Boolean ok;

	main_bundle = CFBundleGetMainBundle();
	if (main_bundle == NULL) goto failed_early;
	resources_url = CFBundleCopyBundleURL(main_bundle);
	if (resources_url == NULL) goto failed_early;
	resources_str = CFURLCopyFileSystemPath(resources_url, kCFURLPOSIXPathStyle);
	if (resources_str == NULL) goto failed_after_resources_url;
	ok = CFStringGetCString(resources_str, buf, FILENAME_MAX, kCFStringEncodingUTF8);
	if (!ok) goto failed_after_resources_str;

	buf[FILENAME_MAX] = '\0';
	ret = path(buf) / "Contents/Resources";

failed_after_resources_str:
	CFRelease(resources_str);
failed_after_resources_url:
	CFRelease(resources_url);
failed_early:
	return ret;
}
#endif

}

namespace paths {

void set_argv_zero(const char* argv_zero) {
	g_argv_zero = argv_zero;
}

path executed_file() {
	path p;

#ifdef __linux__
	if (p.empty()) p = get_executed_file_from_proc();
#endif

#ifdef _WIN32
	if (p.empty()) p = get_executed_file_from_win32_api();
#endif

	if (p.empty()) p = path(g_argv_zero);
	if (p.empty()) p = path("snygg"); // Fail. Give a stupid default

	return p;
}

path executable_path() {
	return executed_file().branch_path(); //< remove "snygg"
}

bool is_installed() {
	path p(executable_path());

	if (p.filename() == "platform-desktop-sdl") return false;

	return true;
}

path guess_prefix() {
	path configured_prefix(INSTALL_PREFIX);

	path p(executable_path());
	if (p.filename() == "games" || p.filename() == "bin") return p.branch_path();
	else return configured_prefix;
}

path guess_project_dir() {
	path p(executable_path());
	if (p.filename() == "platform-desktop-sdl") p = p.branch_path();
	if (p.filename() == "src") p = p.branch_path();
	if (p.filename() == "debug" || p.filename() == "release") p = p.branch_path();
	if (p.filename().string().substr(0, 5) == "build") p = p.branch_path();
	return p;
}

path data() {
	path dir;

#ifdef __MACH__
	if (dir.empty()) dir = get_bundle_resources_directory();
#endif

	if (dir.empty()) {
		if (is_installed()) {
#ifdef _WIN32
			dir = executable_path();
#else
			dir = guess_prefix() / "share/games/snygg";
#endif
		}
		else dir = guess_project_dir();
	}

	return dir;
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
		in.open(try_path.string().c_str());
		if (in.is_open()) return try_path;
	}

	in.open(requested_path.string().c_str());
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
