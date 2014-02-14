#include "lua_vm.hpp"
#include <lua.hpp>

#include <boost/filesystem/path.hpp>

#include "lua_vec.hpp"
#include "lua_segment_heap.hpp"

namespace luamod {

static void register_modules(lua_State* L) {
	load_vec2f(L);
	load_segment_heap(L);
}

lua_vm::lua_vm() {
	L = luaL_newstate();
	luaL_openlibs(L);

	register_modules(L);
}

lua_vm::~lua_vm() {
	lua_close(L);
}

void lua_vm::dofile(const std::string& filename) {
	boost::filesystem::path p(filename);
	std::string package_path(p.branch_path().string());

	lua_getglobal(L, "package");
	lua_pushstring(L, (package_path + "/?.lua").c_str());
	lua_setfield(L, -2, "path");
	lua_pop(L, 1);

	int result = luaL_dofile(L, filename.c_str());
	if (result != LUA_OK) {
		std::string error = lua_tostring(L, -1);
		lua_settop(L, 0);

		if (result == LUA_ERRRUN) {
			std::cerr << "[calling do_file] LUA_ERRRUN: " << error << std::endl;
		} else {
			std::cerr << "[calling do_file] Error(" << result << "): " << error << std::endl;
		}

		throw std::runtime_error(error);
	}
	lua_settop(L, 0);
}

lua_State* lua_vm::get_L() {
	return L;
}

}
