#include <cmath>
#include <memory>
#include <sstream>
#include <vector>
#include <ymse/lean_windows.h>
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>
#include <ymse/rect.hpp>
#include <vec.hpp>
#include "arc.hpp"
#include "line.hpp"
#include "segment_sequence.hpp"
#include "complex_polygon.hpp"
#include "segment_heap.hpp"
#include "complex_polygon_triangulator_gpc.hpp"
#include "lua_vm.hpp"
#include "board.hpp"


struct board::impl {
	std::vector<std::unique_ptr<segment>> b;
	complex_polygon floor_poly;

	luamod::lua_vm lvm;
};


board::board(const boost::filesystem::path& filename) :
	d(new impl)
{
	std::unique_ptr<segment_heap> heap;

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

la::vec2f board::get_starting_position()
{
	try  {
		return la::vec2f(*luabind::call_function<la::vec2f*>(d->lvm.get_L(), "get_starting_position") [
			luabind::adopt(luabind::result)
        ]);
	}
	catch (const luabind::error& e) {
		return la::vec2f(0, -40);
	}
}

void board::render(skin& s) const {
	render_sequence(d->b, s, 0);
}

bool board::intersect_with_circle(const la::vec2f& p, float r) const {
	for (auto& seg : d->b) {
		if (seg->intersect_with_circle(p, r)) return true;
	}
	return false;
}

ymse::rectf board::bounding_box() const {
	ymse::rectf bb = d->b.front()->bounding_box();

	for (auto& seg : d->b) {
		bb = ymse::bounding_box(bb, seg->bounding_box());
	}

	return bb;
}

int board::winding_number(la::vec2f p) const {
	int n = 0;

	for (auto& seg : d->b) {
		n += seg->left_hline_intersections(p);
	}

	return n;
}

void board::calculate_floor_poly() {
	complex_polygon_triangulator_gpc triangulator;
	for (auto& seg : d->b) seg->add_to_triangulator(triangulator);
	d->floor_poly = triangulator.get_complex_polygon();
}

const complex_polygon& board::floor_polygon() const {
	return d->floor_poly;
}
