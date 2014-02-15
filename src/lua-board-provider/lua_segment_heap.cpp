#include <new>
#include <lua.hpp>
#include <vec.hpp>
#include <segment_heap.hpp>


static void check_argcount(lua_State* L, int args, const char* msg) {
	if (lua_gettop(L) != args) luaL_error(L, msg);
}

static segment_heap* self(lua_State* L) {
	luaL_checkudata(L, 1, "segment_heap");
	return static_cast<segment_heap*>(lua_touserdata(L, 1));
}

template <typename ... Args>
static segment_heap* emplace(lua_State* L, Args&& ... args) {
	void *userdata = lua_newuserdata(L, sizeof(segment_heap));
	auto v = new (userdata) segment_heap{args...};
	luaL_setmetatable(L, "segment_heap");
	return v;
}


static int arc(lua_State* L) {
	auto args = lua_gettop(L);
	if (args != 5 && args != 6) {
		lua_pushstring(L, "Incorrect number of args");
		lua_error(L);
	}

	auto heap = self(L);

	float x, y;
	int restargs;
	if (args == 5) {
		luaL_checkudata(L, 2, "vec");
		restargs = 3;

		auto v = static_cast<la::vec2f*>(lua_touserdata(L, 2));
		x = v->x();
		y = v->y();
	} else {
		luaL_checknumber(L, 2);
		luaL_checknumber(L, 3);
		restargs = 4;

		x = lua_tonumber(L, 2);
		y = lua_tonumber(L, 3);
	}

	luaL_checknumber(L, restargs + 0);
	luaL_checknumber(L, restargs + 1);
	luaL_checknumber(L, restargs + 2);

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

	auto heap = self(L);

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

	heap->line(a, b);

	return 0;
}

static int create(lua_State* L) {
	// arg 1 is a table. Which table is this?

	check_argcount(L, 1, "Signature: segment_heap()");

	emplace(L);

	return 1;
}

static int destruct(lua_State* L) {
	check_argcount(L, 1, "Internal error. __gc was called without exactly one argument");
	self(L)->~segment_heap();
	return 0;
}


static void l_set(lua_State* L, int table, const char* key) {
	lua_pushstring(L, key);
	lua_pushvalue(L, -2);
	if (table < 0) --table;
	lua_settable(L, table);
	lua_pop(L, 1);
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

	lua_pushvalue(L, methods);
	l_set(L, metatable, "__index");

	lua_pushcfunction(L, &destruct);
	l_set(L, metatable, "__gc");

	lua_newtable(L);
	lua_pushcfunction(L, &create);
	lua_pushvalue(L, -1);
	l_set(L, methods, "new");
	l_set(L, -3, "__call");
	lua_setmetatable(L, methods);

	luaL_Reg funcs[] = {
		{ "arc", &arc },
		{ "line", &line },
		{ nullptr, nullptr }
	};
	lua_pushvalue(L, methods);
	luaL_setfuncs(L, funcs, 0);
	lua_pop(L, 1);

	lua_pop(L, 2);
}

}
