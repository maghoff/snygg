#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include "lua_vm.hpp"

// For exposing to lua:
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "../segment.hpp"
#include "../line.hpp"
#include "../arc.hpp"
#include "../segment_sequence.hpp"

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

segment_sequence* box(const rectw& rc, float r) {
	std::auto_ptr<segment_sequence> s(new segment_sequence);

	const float &x1 = rc.x1, &y1 = rc.y1, &x2 = rc.x2, &y2 = rc.y2;
	float w = rc.width(), h = rc.height();

	s->push_back(new line(x1 + r, y1    ,  1,  0, w - 2*r));
	s->push_back(new arc (x2 - r, y1 + r,  r,  M_PI * -0.5, M_PI *  0.0, 1));
	s->push_back(new line(x2    , y1 + r,  0,  1, h - 2*r));
	s->push_back(new arc (x2 - r, y2 - r,  r,  M_PI *  0.0, M_PI *  0.5, 1));
	s->push_back(new line(x2 - r, y2    , -1,  0, w - 2*r));
	s->push_back(new arc (x1 + r, y2 - r,  r,  M_PI *  0.5, M_PI *  1.0, 1));
	s->push_back(new line(x1    , y2 - r,  0, -1, h - 2*r));
	s->push_back(new arc (x1 + r, y1 + r,  r,  M_PI * -1.0, M_PI * -0.5, 1));

	return s.release();
}

void register_modules(lua_State* L) {
	using namespace luabind;
	module(L) [
		class_<rectw>("rect")
			.def(constructor<float, float, float, float>())
		,

		class_<ymse::vec2f>("vec")
			.def(constructor<float, float>())
		,

		def("box", &box, adopt(result)),

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
	];
}

}

lua_vm::lua_vm() {
	L = lua_open();
	luaopen_base(L);
	luaopen_string(L);
	luaopen_math(L);
	luabind::open(L);

	register_modules(L);
}

lua_vm::~lua_vm() {
	lua_close(L);
}

void lua_vm::dofile(const std::string& filename) {
	luaL_dofile(L, filename.c_str());
}

lua_State* lua_vm::get_L() {
	return L;
}

}
