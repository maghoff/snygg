#include <cmath>
#include <memory>
#include <sstream>
#include <vector>
#include <lean_windows.h>
#include <lua.hpp>
#include <rect.hpp>
#include <vec.hpp>
#include "arc.hpp"
#include "line.hpp"
#include "segment_sequence.hpp"
#include "complex_polygon.hpp"
#include "segment_heap.hpp"
#include "complex_polygon_triangulator_gpc.hpp"
#include "lua-support/lua_vm.hpp"
#include "board.hpp"


struct board::impl {
	std::vector<std::unique_ptr<segment>> b;
	complex_polygon floor_poly;

	luamod::lua_vm lvm;
};


board::board(const boost::filesystem::path& filename) :
	d(new impl)
{
	d->lvm.dofile(filename.string());

	lua_State *L = d->lvm.get_L();
	lua_getglobal(L, "create_board");
	auto result = lua_pcall(L, 0, 1, 0);
	if (result != LUA_OK) {
		if (result == LUA_ERRRUN) {
			std::cerr << "[calling create_board] Runtime error: " << lua_tostring(L, -1) << std::endl;
		} else {
			std::cerr << "[calling create_board]  result: " << result << std::endl;
		}
		throw new std::runtime_error("Stuff");
	}
	lua_getfield(L, -1, "segment_heap");
	luaL_checkudata(L, -1, "segment_heap");
	auto heap = static_cast<segment_heap*>(lua_touserdata(L, -1));

	d->b.push_back(heap->to_segment());
	lua_pop(L, 2);
	calculate_floor_poly();
}

board::~board() {
}

la::vec2f board::get_starting_position() {
	lua_State *L = d->lvm.get_L();
	lua_getglobal(L, "get_starting_position");
	int result = lua_pcall(L, 0, 1, 0);
	if (result != LUA_OK) {
		if (result == LUA_ERRRUN) {
			std::cerr << "[calling get_starting_position] Runtime error: " << lua_tostring(L, -1) << std::endl;
		} else {
			std::cerr << "[calling get_starting_position]  result: " << result << std::endl;
		}
		return la::vec2f(0, -40);
	}
	luaL_checkudata(L, -1, "vec");
	la::vec2f v = *static_cast<la::vec2f*>(lua_touserdata(L, -1));
	lua_pop(L, 1);
	return v;
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

rectf board::bounding_box() const {
	rectf bb = d->b.front()->bounding_box();

	for (auto& seg : d->b) {
		bb = ::bounding_box(bb, seg->bounding_box());
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
