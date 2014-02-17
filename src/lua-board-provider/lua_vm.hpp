#ifndef LUAMOD_LUA_VM_HPP
#define LUAMOD_LUA_VM_HPP

#include <string>

struct lua_State;
class file_loader;

namespace luamod {

class lua_vm {
	file_loader& loader;
	lua_State* L;

public:
	lua_vm(file_loader&);
	~lua_vm();

	void dofile(const std::string&);

	lua_State* get_L();
};

}

#endif // LUAMOD_LUA_VM_HPP
