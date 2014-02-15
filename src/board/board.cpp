#include "board.hpp"
#include <cmath>
#include <memory>
#include <sstream>
#include <vector>
#include <lean_windows.h>
#include <rect.hpp>
#include <vec.hpp>
#include "arc.hpp"
#include "line.hpp"
#include "segment_sequence.hpp"
#include "complex_polygon.hpp"
#include "segment_heap.hpp"
#include "complex_polygon_triangulator_gpc.hpp"
#include "board_provider.hpp"


struct board::impl {
	board_provider& provider;
	std::vector<std::unique_ptr<segment>> b;
	complex_polygon floor_poly;

	impl(board_provider& provider_) : provider(provider_) {}
};


board::board(board_provider& provider) :
	d(new impl(provider))
{
	d->b.push_back(std::move(d->provider.create_board()));
	calculate_floor_poly();
}

board::~board() {
}

la::vec2f board::get_starting_position() {
	return d->provider.get_starting_position();
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
