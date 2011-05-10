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
		class_<rectw>("rect")
			.def(constructor<float, float, float, float>())
			.def("top", &rectw::top)
			.def("bottom", &rectw::bottom)
			.def("left", &rectw::left)
			.def("right", &rectw::right)
			.def("width", &rectw::width)
			.def("height", &rectw::height)
			.def_readwrite("x1", &rectw::x1)
			.def_readwrite("y1", &rectw::y1)
			.def_readwrite("x2", &rectw::x2)
			.def_readwrite("y2", &rectw::y2)
		,

		class_<ymse::vec2f>("vec")
			.def(constructor<float, float>())
			.def("x", (float(ymse::vec2f::*)() const)&ymse::vec2f::x)
			.def("y", (float(ymse::vec2f::*)() const)&ymse::vec2f::y)
			.def("length", &ymse::vec2f::length)
			.def(const_self + const_self)
			.def(const_self * float())
			.def(float() * const_self)
		,

		def("x", &x),
		def("y", &y),

		class_<segment_heap>("segment_heap")
			.def(constructor<>())
			.def("line", (void(segment_heap::*)(float, float, float, float))&segment_heap::line)
			.def("arc", (void(segment_heap::*)(float, float, float, float, float))&segment_heap::arc)
			.def("to_segment", &segment_heap::to_segment)
		,

		class_<segment>("segment")
			.def("get_head_pos", &segment::get_head_pos)
			.def("get_tail_pos", &segment::get_tail_pos)
		,

		class_<line, segment>("line")
			.def(constructor<ymse::vec2f, ymse::vec2f, float>())
		,

		class_<arc, segment>("arc")
			.def(constructor<ymse::vec2f, float, float, float, float>())
		,

		class_<segment_sequence, segment>("segment_sequence")
			.def(constructor<>())
			.def("push_back",
				 (void(segment_sequence::*)(segment*))&segment_sequence::push_back,
				 adopt(_2)
			)
		,

		class_<open_segment, segment>("open_segment")
			.def(constructor<segment*>(), adopt(_2)),

		class_<contour_segment, segment>("contour")
			.def(constructor<segment*>(), adopt(_2)),

		class_<reverse_contour_segment, segment>("reverse_contour")
			.def(constructor<segment*>(), adopt(_2))
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
