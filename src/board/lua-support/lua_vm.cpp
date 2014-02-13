#include "lua_vm.hpp"
#include <lua.hpp>

#include <boost/filesystem/path.hpp>

#include "vec2f.hpp"
#include "segment_heap.hpp"

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
		if (result == LUA_ERRRUN) {
			std::cerr << "[calling do_file] Runtime error: " << lua_tostring(L, -1) << std::endl;
		} else {
			std::cerr << "[calling do_file]  result: " << result << std::endl;
		}
		throw new std::runtime_error("Stuff");
	}
}

lua_State* lua_vm::get_L() {
	return L;
}

}
