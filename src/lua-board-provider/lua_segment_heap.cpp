#include <new>
#include <lua.hpp>
#include <vec.hpp>
#include <segment_heap.hpp>

static void l_set(lua_State* L, int table, const char* key) {
	lua_pushstring(L, key);
	lua_pushvalue(L, -2);
	if (table < 0) --table;
	lua_settable(L, table);
	lua_pop(L, 1);
}

static int arc(lua_State* L) {
	auto args = lua_gettop(L);
	if (args != 5 && args != 6) {
		lua_pushstring(L, "Incorrect number of args");
		lua_error(L);
	}

	luaL_checkudata(L, 1, "segment_heap");

	float x, y;
	int restargs;
	if (args == 5) {
		luaL_checkudata(L, 2, "vec");
		luaL_checknumber(L, 3);
		luaL_checknumber(L, 4);
		luaL_checknumber(L, 5);

		auto v = static_cast<la::vec2f*>(lua_touserdata(L, 2));
		x = v->x();
		y = v->y();
		restargs = 3;
	} else {
		luaL_checknumber(L, 2);
		luaL_checknumber(L, 3);
		luaL_checknumber(L, 4);
		luaL_checknumber(L, 5);
		luaL_checknumber(L, 6);

		x = lua_tonumber(L, 2);
		y = lua_tonumber(L, 3);
		restargs = 4;
	}

	auto heap = static_cast<segment_heap*>(lua_touserdata(L, 1));
	heap->arc(
		x, y,
		lua_tonumber(L, restargs + 0),
		lua_tonumber(L, restargs + 1),
		lua_tonumber(L, restargs + 2)
	);

	return 0;
}

static int line(lua_State* L) {
	auto args = lua_gettop(L);
	if (args != 5 && args != 6) {
		lua_pushstring(L, "Incorrect number of args");
		lua_error(L);
	}

	luaL_checkudata(L, 1, "segment_heap");

	la::vec2f a, b;
	if (args == 3) {
		luaL_checkudata(L, 2, "vec");
		luaL_checkudata(L, 3, "vec");

		a = *static_cast<la::vec2f*>(lua_touserdata(L, 2));
		b = *static_cast<la::vec2f*>(lua_touserdata(L, 3));
	} else {
		luaL_checknumber(L, 2);
		luaL_checknumber(L, 3);
		luaL_checknumber(L, 4);
		luaL_checknumber(L, 5);

		a.x() = lua_tonumber(L, 2);
		a.y() = lua_tonumber(L, 3);
		b.x() = lua_tonumber(L, 4);
		b.y() = lua_tonumber(L, 5);
	}

	auto heap = static_cast<segment_heap*>(lua_touserdata(L, 1));
	heap->line(a, b);

	return 0;
}

static int create(lua_State* L) {
	// arg 1 is a table. Which table is this?

	// Assert argument count

	void *userdata = lua_newuserdata(L, sizeof(segment_heap));
	new (userdata) segment_heap;

	luaL_setmetatable(L, "segment_heap");

	return 1;
}

static int destruct(lua_State* L) {
	// Assert argument count
	luaL_checkudata(L, 1, "segment_heap");
	auto heap = static_cast<segment_heap*>(lua_touserdata(L, 1));

	heap->~segment_heap();

	return 0;
}


namespace luamod {

void load_segment_heap(lua_State* L) {
	lua_newtable(L);
	int methods = lua_gettop(L);
	luaL_newmetatable(L, "segment_heap");
	int metatable = lua_gettop(L);
	lua_pushvalue(L, methods);
	lua_setglobal(L, "segment_heap");

	lua_pushvalue(L, methods);  
	l_set(L, metatable, "__metatable");

	//set metatable __index
	lua_pushvalue(L, methods);
	l_set(L, metatable, "__index");

	//set metatable __gc
	lua_pushcfunction(L, &destruct);
	l_set(L, metatable, "__gc");

	//set method table
	lua_newtable(L);                // mt for method table  
	lua_pushcfunction(L, &create);
	lua_pushvalue(L, -1);           // dup new_T function  
	l_set(L, methods, "new");         // add new_T to method table  
	l_set(L, -3, "__call");           // mt.__call = new_T  
	lua_setmetatable(L, methods);  

	// set methods metatable   
	lua_pushstring(L, "arc");
	lua_pushcfunction(L, &arc);  
	lua_settable(L, methods);

	lua_pushstring(L, "line");
	lua_pushcfunction(L, &line);
	lua_settable(L, methods);

	lua_pop(L, 2);
}

}
