#include <iostream>

#include <SDL.h>
#include <cmath>
#include <boost/ptr_container/ptr_vector.hpp>
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "arc.hpp"
#include "line.hpp"
#include "luamod/lua_vm.hpp"
#include "board.hpp"
#include "segment_sequence.hpp"


struct board::impl {
	boost::ptr_vector<segment> b;

	luamod::lua_vm lvm;
};


board::board(const std::string& filename) :
	d(new impl)
{
	try  {
		d->lvm.dofile(filename);

		d->b.push_back(
			luabind::call_function<segment*>(d->lvm.get_L(), "create_board") [
				luabind::adopt(luabind::result)
			]
		);
	}
	catch (const luabind::error& e) {
		luabind::object error_msg(luabind::from_stack(e.state(), -1));
		std::cerr << "Lua error: " << error_msg << std::endl << std::endl;

		// This is a bit drastic, but the luabind exception mechanism is cramping my style:
		exit(-1);
	}
}

board::~board() {
}

void board::render(skin& s) const {
	render_sequence(d->b, s, SDL_GetTicks() * 0.0002);
}

bool board::intersect_with_circle(const ymse::vec2f& p, float r) const {
	typedef boost::ptr_vector<segment>::const_iterator iter;
	iter end = d->b.end();
	for (iter i = d->b.begin(); i != end; ++i) {
		if (i->intersect_with_circle(p, r)) return true;
	}
	return false;
}

ymse::rectf board::bounding_box() const {
	ymse::rectf bb = d->b[0].bounding_box();

	typedef boost::ptr_vector<segment>::const_iterator iter;
	iter end = d->b.end();
	for (iter i = d->b.begin(); i != end; ++i) {
		bb = ymse::bounding_box(bb, i->bounding_box());
	}

	return bb;
}

int board::winding_number(ymse::vec2f p) const {
	int n = 0;

	typedef boost::ptr_vector<segment>::const_iterator iter;
	iter end = d->b.end();
	for (iter i = d->b.begin(); i != end; ++i) {
		n += i->left_hline_intersections(p);
	}

	return n;
}
