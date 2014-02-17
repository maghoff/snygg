#include "lua_vm.hpp"
#include <stdexcept>
#include <iostream>
#include <lua.hpp>
#include "lua_vec.hpp"
#include "lua_segment_heap.hpp"
#include "file_loader.hpp"
#include "istream_lua_load.hpp"

namespace luamod {

static void register_modules(lua_State* L) {
	load_vec2f(L);
	load_segment_heap(L);
}

static int loader(lua_State* L) {
	auto name = luaL_checkstring(L, 1);
	auto& in = *static_cast<std::unique_ptr<std::istream>*>(lua_touserdata(L, 2));

	auto load_result = istream_lua_load(L, *in, name);
	if (load_result != LUA_OK) lua_error(L);

	in.reset();

	lua_call(L, 0, 1);

	return 1;
}

static int destruct_istream(lua_State* L) {
	auto in = static_cast<std::unique_ptr<std::istream>*>(lua_touserdata(L, 1));
	using std::unique_ptr;
	in->~unique_ptr<std::istream>();
	return 0;
}

static int searcher(lua_State* L) {
	auto& floader = **static_cast<file_loader**>(lua_touserdata(L, lua_upvalueindex(1)));

	size_t len;
	const char* name_raw = luaL_checklstring(L, 1, &len);
	std::string name(name_raw, len);

	try {
		auto in = floader.open_file(name);

		lua_pushcfunction(L, &loader);

		void* userdata = lua_newuserdata(L, sizeof(std::unique_ptr<std::istream>));
		new (userdata) (std::unique_ptr<std::istream>){std::move(in)};
		lua_newtable(L);
		lua_pushcfunction(L, &destruct_istream);
		lua_setfield(L, -2, "__gc");
		lua_setmetatable(L, -2);

		return 2;
	}
	catch (file_not_found& err) {
		lua_pushstring(L, err.what());
		return 1;
	}
}

lua_vm::lua_vm(file_loader& loader_) :
	loader(loader_)
{
	L = luaL_newstate();
	luaL_openlibs(L);

	register_modules(L);

	lua_getglobal(L, "package");
	lua_createtable(L, 1, 0);
	lua_pushnumber(L, 1);
	void* userdata = lua_newuserdata(L, sizeof(file_loader*));
	new (userdata) (file_loader*){&loader};
	lua_pushcclosure(L, &searcher, 1);
	lua_settable(L, -3);
	lua_setfield(L, -2, "searchers");
	lua_pop(L, 1);
}

lua_vm::~lua_vm() {
	lua_close(L);
}

void lua_vm::dofile(const std::string& name) {
	std::unique_ptr<std::istream> in = loader.open_file(name);
	auto load_result = istream_lua_load(L, *in, name);
	if (load_result != LUA_OK) {
		std::string error = lua_tostring(L, -1);
		lua_pop(L, 1);

		throw std::runtime_error(error);
	}

	int result = lua_pcall(L, 0, 0, 0);
	if (result != LUA_OK) {
		std::string error = lua_tostring(L, -1);
		lua_pop(L, 1);

		if (result == LUA_ERRRUN) {
			std::cerr << "[calling do_file] LUA_ERRRUN: " << error << std::endl;
		} else {
			std::cerr << "[calling do_file] Error(" << result << "): " << error << std::endl;
		}

		throw std::runtime_error(error);
	}
}

lua_State* lua_vm::get_L() {
	return L;
}

}
