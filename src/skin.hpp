#ifndef SNYGG_SKIN_HPP
#define SNYGG_SKIN_HPP

namespace ymse {
	template <int dim, typename T>
	class vec;

	typedef vec<2, float> vec2f;

	template <typename T>
	struct rect;

	typedef rect<float> rectf;
}

class skin {
public:
	virtual ~skin();

	virtual void circle(ymse::vec2f p, float r) = 0;
	virtual void blood(ymse::vec2f p, float r) = 0;

	virtual void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) = 0;
	virtual void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end) = 0;

	virtual void cap_front(ymse::vec2f p, float direction, float b_coord) = 0;
	virtual void cap_back(ymse::vec2f p, float direction, float b_coord) = 0;

	// Give the skin opportunity to put on the last finish:
	virtual void finish_frame(ymse::rectf bounding_box) = 0;
};

#endif
