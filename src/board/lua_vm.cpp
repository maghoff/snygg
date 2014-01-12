#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/operator.hpp>
#include "lua_vm.hpp"

#include <boost/filesystem/path.hpp>

// For exposing to lua:
#include <ymse/vec.hpp>
#include "segment_heap.hpp"

namespace luamod {

namespace {

void register_modules(lua_State* L) {
	using namespace luabind;
	module(L) [
		class_<ymse::vec2f>("vec")
			.def(constructor<float, float>())
			.def("x", (float(ymse::vec2f::*)() const)&ymse::vec2f::x)
			.def("y", (float(ymse::vec2f::*)() const)&ymse::vec2f::y)
			.def("length", &ymse::vec2f::length)
			.def(const_self + const_self)
			.def(const_self * float())
			.def(float() * const_self)
		,

		class_<segment_heap>("segment_heap")
			.def(constructor<>())
			.def("line", (void(segment_heap::*)(float, float, float, float))&segment_heap::line)
			.def("line", (void(segment_heap::*)(ymse::vec2f, ymse::vec2f))&segment_heap::line)
			.def("arc", (void(segment_heap::*)(float, float, float, float, float))&segment_heap::arc)
			.def("arc", (void(segment_heap::*)(ymse::vec2f, float, float, float))&segment_heap::arc)
	];
}

}

lua_vm::lua_vm() {
	L = lua_open();
	luaL_openlibs(L);
	luabind::open(L);

	register_modules(L);
}

lua_vm::~lua_vm() {
	lua_close(L);
}

void lua_vm::dofile(const std::string& filename) {

	boost::filesystem::path p(filename);
	std::string package_path(p.branch_path().string());

	luabind::object package = luabind::globals(L)["package"];
	package["path"] = package_path + "/?.lua";

	int result = luaL_dofile(L, filename.c_str());
	if (result) throw luabind::error(L);
}

lua_State* lua_vm::get_L() {
	return L;
}

}
