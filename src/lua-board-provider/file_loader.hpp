#ifndef LUA_BOARD_PROVIDER_FILE_LOADER_HPP
#define LUA_BOARD_PROVIDER_FILE_LOADER_HPP

#include <stdexcept>
#include <iosfwd>
#include <string>
#include <memory>

class file_not_found : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

class file_loader {
public:
	file_loader() = default;
	virtual ~file_loader() = default;

	file_loader(const file_loader&) = delete;
	file_loader& operator = (const file_loader&) = delete;

	virtual std::unique_ptr<std::istream> open_file(const std::string& name) = 0;
};

#endif
