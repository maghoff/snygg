#include <new>
#include <type_traits>
#include <lua.hpp>
#include <vec.hpp>


static void check_argcount(lua_State* L, int args, const char* msg) {
	if (lua_gettop(L) != args) luaL_error(L, msg);
}

static la::vec2f* self(lua_State* L) {
	luaL_checkudata(L, 1, "vec");
	return static_cast<la::vec2f*>(lua_touserdata(L, 1));
}

template <typename ... Args>
static la::vec2f* emplace(lua_State* L, Args&& ... args) {
	void *userdata = lua_newuserdata(L, sizeof(la::vec2f));
	auto v = new (userdata) la::vec2f{args...};
	luaL_setmetatable(L, "vec");
	return v;
}


static int x(lua_State* L) {
	check_argcount(L, 1, "Signature: vec:x()");
	lua_pushnumber(L, self(L)->x());
	return 1;
}

static int y(lua_State* L) {
	check_argcount(L, 1, "Signature: vec:y()");
	lua_pushnumber(L, self(L)->y());
	return 1;
}

static int operator_add(lua_State* L) {
	check_argcount(L, 2, "Signature: __add(vec, vec)");

	luaL_checkudata(L, 1, "vec");
	luaL_checkudata(L, 2, "vec");
	la::vec2f* lhs = static_cast<la::vec2f*>(lua_touserdata(L, 1));
	la::vec2f* rhs = static_cast<la::vec2f*>(lua_touserdata(L, 2));

	emplace(L, (*lhs) + (*rhs));

	return 1;
}

static int create(lua_State* L) {
	// arg 1 is a table. Which table is this?

	check_argcount(L, 3, "Signature: vec(number, number)");

	auto x = static_cast<float>(luaL_checknumber(L, 2));
	auto y = static_cast<float>(luaL_checknumber(L, 3));
	emplace(L, x, y);

	return 1;
}


static void l_set(lua_State* L, int table, const char* key) {
	lua_pushstring(L, key);
	lua_pushvalue(L, -2);
	if (table < 0) --table;
	lua_settable(L, table);
	lua_pop(L, 1);
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

	lua_pushvalue(L, methods);
	l_set(L, metatable, "__index");

	//don't set metatable __gc, since we have a POD type
	static_assert(std::is_pod<la::vec2f>::value, "vec2f needs its destructor called if it is not a POD type");

	lua_pushcfunction(L, &operator_add);
	l_set(L, metatable, "__add");

	//set method table
	lua_newtable(L);
	lua_pushcfunction(L, &create);
	lua_pushvalue(L, -1);
	l_set(L, methods, "new");
	l_set(L, -3, "__call");
	lua_setmetatable(L, methods);

	// set methods metatable
	lua_pushstring(L, "x");
	lua_pushcfunction(L, &x);
	lua_settable(L, methods);

	lua_pushstring(L, "y");
	lua_pushcfunction(L, &y);
	lua_settable(L, methods);

	lua_pop(L, 2);
}

}
