#ifndef SNYGG_SKIN_HPP
#define SNYGG_SKIN_HPP

#include <ymse/vec_fwd.hpp>

class complex_polygon;

class skin {
public:
	enum state_t {
		other_state,
		board_state
	};

	virtual ~skin();

	virtual void load_opengl_resources(int width, int height);
	
	virtual void circle(ymse::vec2f p, float r) = 0;
	virtual void blood(ymse::vec2f p, float r) = 0;

	virtual void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) = 0;
	virtual void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end) = 0;

	virtual void cap(ymse::vec2f p, float snake_direction, float cap_direction, float b_coord) = 0;

	virtual void floor(const complex_polygon&) = 0;

	virtual void enter_state(state_t) { }
};

#endif
