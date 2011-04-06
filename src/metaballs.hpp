#ifndef METABALLS_HPP
#define METABALLS_HPP

#include <boost/scoped_ptr.hpp>
#include <vector>

namespace ymse {
	typedef vec<3, float> vec3f;

	template <typename T>
	struct rect;

	typedef rect<float> rectf;
}

template <class BaseSkin>
class metaballs : public BaseSkin {
	struct impl;
	boost::scoped_ptr<impl> d;

	void init(const std::string& path);

	// Three coordinates: x, y and size
	void render_metaballs(ymse::rectf rc, const std::vector<ymse::vec3f>&);

public:
	metaballs(const std::string& path);
	~metaballs();

	void load_opengl_resources();

	void blood(ymse::vec2f p, float r);

	void finish_frame(ymse::rectf bb);
};

#endif // METABALLS_HPP
