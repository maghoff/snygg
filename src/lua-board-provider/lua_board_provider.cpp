#include "lua_board_provider.hpp"
#include <stdexcept>
#include <iostream>
#include <lua.hpp>
#include <vec.hpp>
#include <segment.hpp>
#include <segment_heap.hpp>
#include "file_loader.hpp"
#include "lua_vm.hpp"

struct lua_board_provider::impl {
	luamod::lua_vm lvm;

	impl(file_loader& loader) : lvm(loader) {}
};

lua_board_provider::lua_board_provider(file_loader& loader, const std::string& name) :
	d(new impl(loader))
{
	d->lvm.dofile(name);
}

lua_board_provider::~lua_board_provider() {
}

std::unique_ptr<segment> lua_board_provider::create_board() {
	lua_State *L = d->lvm.get_L();
	lua_getglobal(L, "create_board");
	auto result = lua_pcall(L, 0, 1, 0);
	if (result != LUA_OK) {
		std::string error = lua_tostring(L, -1);
		lua_pop(L, 1);
		if (result == LUA_ERRRUN) {
			std::cerr << "[calling create_board] LUA_ERRUN: " << error << std::endl;
		} else {
			std::cerr << "[calling create_board] Error(" << result << "): " << error << std::endl;
		}
		throw std::runtime_error(error);
	}
	lua_getfield(L, -1, "segment_heap");
	luaL_checkudata(L, -1, "segment_heap");
	auto heap = static_cast<segment_heap*>(lua_touserdata(L, -1));
	auto board = heap->to_segment();
	lua_pop(L, 2);

	return board;
}

la::vec2f lua_board_provider::get_starting_position() {
	lua_State *L = d->lvm.get_L();
	lua_getglobal(L, "get_starting_position");
	int result = lua_pcall(L, 0, 1, 0);
	if (result != LUA_OK) {
		std::string error = lua_tostring(L, -1);
		lua_pop(L, 1);
		if (result == LUA_ERRRUN) {
			std::cerr << "[calling get_starting_position] LUA_ERRUN: " << error << std::endl;
		} else {
			std::cerr << "[calling get_starting_position] Error(" << result << "): " << error << std::endl;
		}
		return la::vec2f(0, -40);
	}
	luaL_checkudata(L, -1, "vec");
	la::vec2f v = *static_cast<la::vec2f*>(lua_touserdata(L, -1));
	lua_pop(L, 1);
	return v;
}
