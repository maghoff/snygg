#ifndef TEXTURED_SKIN_HPP
#define TEXTURED_SKIN_HPP

#include <vector>
#include <boost/scoped_ptr.hpp>
#include "scalable_skin.hpp"

namespace ymse {
	typedef vec<3, float> vec3f;

	template <typename T>
	struct rect;

	typedef rect<float> rectf;
}

class textured_skin : public scalable_skin {
	struct impl;
	boost::scoped_ptr<impl> d;

	// Three coordinates: x, y and size
	void metaballs(ymse::rectf rc, const std::vector<ymse::vec3f>&);

public:
	textured_skin(const std::string& path);

	void circle(ymse::vec2f p, float r);
	void blood(ymse::vec2f p, float r);

	void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end);

	void finish_frame(ymse::rectf bb);
};

#endif // TEXTURED_SKIN_HPP
