#ifndef METABALLS_HPP
#define METABALLS_HPP

#include <vector>
#include "textured_skin.hpp"

namespace ymse {
	typedef vec<3, float> vec3f;

	template <typename T>
	struct rect;

	typedef rect<float> rectf;
}

class metaballs : public textured_skin {
	struct impl;
	boost::scoped_ptr<impl> d;

	// Three coordinates: x, y and size
	void render_metaballs(ymse::rectf rc, const std::vector<ymse::vec3f>&);

public:
	metaballs(const std::string& path);
	~metaballs();

	void blood(ymse::vec2f p, float r);

	void finish_frame(ymse::rectf bb);
};

#endif // METABALLS_HPP
