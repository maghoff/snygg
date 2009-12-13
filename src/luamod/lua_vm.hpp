#ifndef LUAMOD_LUA_VM_HPP
#define LUAMOD_LUA_VM_HPP

#include <string>

struct lua_State;

namespace luamod {

class lua_vm {
	lua_State* L;
public:
	lua_vm();
	~lua_vm();

	void dofile(const std::string& filename);

	lua_State* get_L();
};

}

#endif // LUAMOD_LUA_VM_HPP
