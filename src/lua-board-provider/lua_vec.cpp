#include <new>
#include <lua.hpp>
#include <vec.hpp>

static void l_set(lua_State* L, int table, const char* key) {
	lua_pushstring(L, key);
	lua_pushvalue(L, -2);
	if (table < 0) --table;
	lua_settable(L, table);
	lua_pop(L, 1);
}

static int getX(lua_State* L) {
	// Assert argument count
	luaL_checkudata(L, 1, "vec");
	la::vec2f* v = static_cast<la::vec2f*>(lua_touserdata(L, 1));
	// Assert !nullptr
	lua_pushnumber(L, v->x());
	return 1;
}

static int getY(lua_State* L) {
	// Assert argument count
	luaL_checkudata(L, 1, "vec");
	la::vec2f* v = static_cast<la::vec2f*>(lua_touserdata(L, 1));
	// Assert !nullptr
	lua_pushnumber(L, v->y());
	return 1;
}

static int operator_add(lua_State* L) {
	// Assert argument count and types
	luaL_checkudata(L, 1, "vec");
	luaL_checkudata(L, 2, "vec");
	la::vec2f* lhs = static_cast<la::vec2f*>(lua_touserdata(L, 1));
	la::vec2f* rhs = static_cast<la::vec2f*>(lua_touserdata(L, 2));
	// Assert !nullptr

	void *userdata = lua_newuserdata(L, sizeof(la::vec2f));
	la::vec2f *res = new (userdata) la::vec2f;
	*res = (*lhs) + (*rhs);

	luaL_setmetatable(L, "vec");

	return 1;
}

static int create(lua_State* L) {
	// arg 1 is a table. Which table is this?

	// Assert argument count
	auto x = static_cast<float>(lua_tonumber(L, 2));
	auto y = static_cast<float>(lua_tonumber(L, 3));

	void *userdata = lua_newuserdata(L, sizeof(la::vec2f));
	new (userdata) la::vec2f{x, y};

	luaL_setmetatable(L, "vec");

	return 1;
}

static int destruct(lua_State* L) {
	// Assert argument count
	luaL_checkudata(L, 1, "vec");
	la::vec2f* v = static_cast<la::vec2f*>(lua_touserdata(L, 1));
	// Assert !nullptr
	using la::vec2f;
	v->~vec2f(); //< NOOP for POD types, such as vec2f

	return 0;
}


namespace luamod {

void load_vec2f(lua_State* L) {
	lua_newtable(L);
	int methods = lua_gettop(L);
	luaL_newmetatable(L, "vec");
	int metatable = lua_gettop(L);
	lua_pushvalue(L, methods);
	lua_setglobal(L, "vec");

	lua_pushvalue(L, methods);  
	l_set(L, metatable, "__metatable");

	//set metatable __index
	lua_pushvalue(L, methods);
	l_set(L, metatable, "__index");

	//set metatable __gc
	lua_pushcfunction(L, &destruct);
	l_set(L, metatable, "__gc");

	//set metatable __add
	lua_pushcfunction(L, &operator_add);
	l_set(L, metatable, "__add");

	//set method table
	lua_newtable(L);                // mt for method table  
	lua_pushcfunction(L, &create);
	lua_pushvalue(L, -1);           // dup new_T function  
	l_set(L, methods, "new");         // add new_T to method table  
	l_set(L, -3, "__call");           // mt.__call = new_T  
	lua_setmetatable(L, methods);  

	// set methods metatable   
	lua_pushstring(L, "x");
	lua_pushcfunction(L, &getX);  
	lua_settable(L, methods);

	lua_pushstring(L, "y");
	lua_pushcfunction(L, &getY);
	lua_settable(L, methods);

	lua_pop(L, 2);
}

}
