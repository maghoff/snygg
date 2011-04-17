#ifndef SNYGG_BOARD_HPP
#define SNYGG_BOARD_HPP

#include <string>
#include <boost/filesystem/path.hpp>
#include <boost/scoped_ptr.hpp>
#include <ymse/vec.hpp>
#include "intersectable_with_circle.hpp"
#include "renderable.hpp"

namespace ymse {
	template <typename T>
	struct rect;

	typedef rect<float> rectf;

	template <int Dim, typename T>
	class vec;

	typedef vec<2, float> vec2f;
}

class board : public renderable, public intersectable_with_circle {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	board(const boost::filesystem::path& filename);
	~board();

	void render(skin&) const;

	bool intersect_with_circle(const ymse::vec2f&, float r) const;

	ymse::vec2f get_starting_position();
	ymse::rectf bounding_box() const;
	int winding_number(ymse::vec2f) const;
};

#endif
