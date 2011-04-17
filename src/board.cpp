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
#include "complex_polygon.hpp"
#include "complex_polygon_triangulator.hpp"
#include "line.hpp"
#include "luamod/lua_vm.hpp"
#include "board.hpp"
#include "segment_sequence.hpp"


struct board::impl {
	boost::ptr_vector<segment> b;
	complex_polygon floor_poly;

	luamod::lua_vm lvm;
};


board::board(const boost::filesystem::path& filename) :
	d(new impl)
{
	try  {
		d->lvm.dofile(filename.string());

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
	complex_polygon_triangulator triangulator;

	triangulator.start_contour();
	typedef boost::ptr_vector<segment>::const_iterator seg_it;
	for (seg_it it = d->b.begin(); it != d->b.end(); ++it) {
		it->add_to_triangulator(&triangulator);
	}
	triangulator.end_contour();

	d->floor_poly = triangulator.get_complex_polygon();
}

const complex_polygon& board::floor_polygon() const {
	return d->floor_poly;
}
