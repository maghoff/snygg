#include "istream_lua_load.hpp"

#include <lua.hpp>
#include <istream>
#include <array>

namespace {

struct reader {
	std::istream& in;
	std::array<char, 4096> buffer;

	const char * read(size_t *size) {
		if (in.eof()) return nullptr;

		in.read(buffer.data(), buffer.size());
		*size = in.gcount();
		return buffer.data();
	}
};

extern "C" {

static const char * read_trampoline(lua_State* L, void *data, size_t *size) {
	return static_cast<reader*>(data)->read(size);
}

}

}


int istream_lua_load(lua_State* L, std::istream& in, const std::string& source, const std::string& mode) {
	reader r = { in };
	return lua_load(L, read_trampoline, static_cast<void*>(&r), source.c_str(), mode.c_str());
}
