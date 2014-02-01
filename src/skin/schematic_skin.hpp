#ifndef SCHEMATIC_SKIN_HPP
#define SCHEMATIC_SKIN_HPP

#include "plain_skin.hpp"

class schematic_skin : public plain_skin {
public:
	schematic_skin();
	~schematic_skin();

	void circle(la::vec2f p, float r);
	void blood(la::vec2f p, float r);

	void fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(la::vec2f p, la::vec2f d, float len, float t, float b_begin, float b_end);

	void cap(la::vec2f p, float snake_direction, float cap_direction, float b_coord);

	void floor(const complex_polygon&);
};

#endif // SCHEMATIC_SKIN_HPP
