#ifndef SNYGG_SKIN_HPP
#define SNYGG_SKIN_HPP

namespace ymse {
	template <int dim, typename T>
	class vec;

	typedef vec<2, float> vec2f;
}

class complex_polygon;

class skin {
public:
	virtual ~skin();

	virtual void load_opengl_resources();
	
	virtual void circle(ymse::vec2f p, float r) = 0;
	virtual void blood(ymse::vec2f p, float r) = 0;

	virtual void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) = 0;
	virtual void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end) = 0;

	virtual void cap(ymse::vec2f p, float snake_direction, float cap_direction, float b_coord) = 0;

	virtual void floor(const complex_polygon&) = 0;
};

#endif
