#ifndef SCHEMATIC_SVG_SKIN_HPP
#define SCHEMATIC_SVG_SKIN_HPP

#include <iostream>
#include <stack>
#include <string>
#include "scalable_skin.hpp"

namespace ymse {
	template <typename T>
	struct rect;

	typedef rect<float> rectf;
}

class schematic_svg_skin : public scalable_skin {
	std::ostream& out;
	std::stack<std::string> output_stack;

public:
	schematic_svg_skin(std::ostream& out, const ymse::rectf& bounding_box);
	~schematic_svg_skin();

	void circle(ymse::vec2f p, float r);
	void blood(ymse::vec2f p, float r);

	void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end);

	void cap(ymse::vec2f p, float snake_direction, float cap_direction, float b_coord);

	void floor(const complex_polygon&);
};

#endif // SCHEMATIC_SVG_SKIN_HPP
