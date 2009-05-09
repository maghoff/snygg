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

	virtual void fat_arc(float x, float y, float r, float t, float begin, float end) = 0;
	virtual void fat_line(float x, float y, float dx, float dy, float len, float t) = 0;
};

#endif
