#ifndef SNYGG_SKIN_HPP
#define SNYGG_SKIN_HPP

#include <vec_fwd.hpp>

struct complex_polygon;

class skin {
public:
	enum state_t {
		other_state,
		board_state,
		floor_state
	};

	virtual ~skin();

	virtual void load_opengl_resources(int width, int height);

	virtual void circle(la::vec2f p, float r) = 0;
	virtual void blood(la::vec2f p, float r) = 0;

	virtual void fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) = 0;
	virtual void fat_line(la::vec2f p, la::vec2f d, float len, float t, float b_begin, float b_end) = 0;

	virtual void cap(la::vec2f p, float snake_direction, float cap_direction, float b_coord) = 0;

	virtual void floor(const complex_polygon&) = 0;

	virtual void enter_state(state_t) { }
};

#endif
