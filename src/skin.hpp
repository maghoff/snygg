#ifndef SNYGG_SKIN_HPP
#define SNYGG_SKIN_HPP

namespace ymse {
	template <int dim, typename T>
	class vec;

	typedef vec<2, float> vec2f;
}

class skin {
public:
	virtual ~skin();

	virtual void circle(ymse::vec2f p, float r) = 0;
	virtual void blood(ymse::vec2f p, float r) = 0;

	virtual void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) = 0;
	virtual void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end) = 0;
};

#endif
