#include <iostream>
#include <sstream>

#include <SDL.h>
#include <cmath>
#include <boost/ptr_container/ptr_vector.hpp>
#include <ymse/lean_windows.h>
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "segment/arc.hpp"
#include "segment/line.hpp"
#include "segment/segment_sequence.hpp"
#include "segment_heap.hpp"
#include "skin/complex_polygon.hpp"
#include "complex_polygon_triangulator_gpc.hpp"
#include "luamod/lua_vm.hpp"
#include "board.hpp"


struct board::impl {
	boost::ptr_vector<segment> b;
	complex_polygon floor_poly;

	luamod::lua_vm lvm;
};


board::board(const boost::filesystem::path& filename) :
	d(new impl)
{
	boost::scoped_ptr<segment_heap> heap;

	try  {
		d->lvm.dofile(filename.string());

		heap.reset(
			luabind::call_function<segment_heap*>(d->lvm.get_L(), "create_board") [
				luabind::adopt(luabind::result)
			]
		);
	}
	catch (const luabind::error& e) {
		luabind::object error_msg(luabind::from_stack(e.state(), -1));
		std::stringstream ss;
		ss << "Lua error: " << error_msg;

		std::cerr << ss.str() << "\n\n" << std::flush;

#ifdef _WIN32
		MessageBox(0, ss.str().c_str(), "Lua error", MB_OK | MB_ICONERROR);
#endif

		// This is a bit drastic, but the luabind exception mechanism is cramping my style:
		exit(-1);
	}

	d->b.push_back(heap->to_segment());
	calculate_floor_poly();
}

board::~board() {
}

ymse::vec2f board::get_starting_position()
{
	try  {
		return ymse::vec2f(*luabind::call_function<ymse::vec2f*>(d->lvm.get_L(), "get_starting_position") [
			luabind::adopt(luabind::result)
        ]);
	}
	catch (const luabind::error& e) {
		return ymse::vec2f(0, -40);
	}
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

void board::calculate_floor_poly() {
	complex_polygon_triangulator_gpc triangulator_impl;

	complex_polygon_triangulator& triangulator = triangulator_impl;

	typedef boost::ptr_vector<segment>::const_iterator seg_it;
	for (seg_it it = d->b.begin(); it != d->b.end(); ++it) {
		it->add_to_triangulator(&triangulator);
	}

	d->floor_poly = triangulator.get_complex_polygon();
}

const complex_polygon& board::floor_polygon() const {
	return d->floor_poly;
}
