#ifndef SNYGG_BOARD_LUA_SUPPORT_SEGMENT_HEAP_HPP
#define SNYGG_BOARD_LUA_SUPPORT_SEGMENT_HEAP_HPP

struct lua_State;

namespace luamod {

void load_segment_heap(lua_State* L);

}

#endif
