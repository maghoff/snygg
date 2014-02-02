#ifndef SNYGG_BOARD_HPP
#define SNYGG_BOARD_HPP

#include <memory>
#include <string>
#include <boost/filesystem/path.hpp>
#include <vec.hpp>
#include "intersectable_with_circle.hpp"
#include "renderable.hpp"

template <typename T> struct rect;
typedef rect<float> rectf;

class complex_polygon;

class board : public renderable, public intersectable_with_circle {
	struct impl;
	std::unique_ptr<impl> d;

	void calculate_floor_poly();

public:
	board(const boost::filesystem::path& filename);
	~board();

	void render(skin&) const;

	bool intersect_with_circle(const la::vec2f&, float r) const;

	la::vec2f get_starting_position();
	rectf bounding_box() const;
	int winding_number(la::vec2f) const;

	const complex_polygon& floor_polygon() const;
};

#endif
