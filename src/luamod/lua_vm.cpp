#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <luabind/operator.hpp>
#include "lua_vm.hpp"

#include <boost/filesystem/path.hpp>

// For exposing to lua:
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "../segment.hpp"
#include "../line.hpp"
#include "../arc.hpp"
#include "../segment_heap.hpp"
#include "../segment_sequence.hpp"
#include "../open_segment.hpp"
#include "../contour_segment.hpp"
#include "../reverse_contour_segment.hpp"

namespace luamod {

namespace {

class rectw : public ymse::rectf {
public:
	rectw(float x1_, float y1_, float x2_, float y2_) {
		x1 = x1_;
		y1 = y1_;
		x2 = x2_;
		y2 = y2_;
	}
};

float x(const ymse::vec2f& v) { return v[0]; }
float y(const ymse::vec2f& v) { return v[1]; }

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
			.def("arc", (void(segment_heap::*)(float, float, float, float, float))&segment_heap::arc)
			.def("to_segment", &segment_heap::to_segment)
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
