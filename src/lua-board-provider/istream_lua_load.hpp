#ifndef ISTREAM_LUA_LOAD_HPP
#define ISTREAM_LUA_LOAD_HPP

#include <iosfwd>
#include <string>

struct lua_State;

int istream_lua_load(lua_State* L, std::istream& in, const std::string& source, const std::string& mode = "bt");

#endif // ISTREAM_LUA_LOAD_HPP
