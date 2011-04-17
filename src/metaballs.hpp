#ifndef METABALLS_HPP
#define METABALLS_HPP

#include <boost/scoped_ptr.hpp>
#include <vector>

namespace ymse {
	template <int Dim, typename T>
	class vec;

	typedef vec<2, float> vec2f;
	typedef vec<3, float> vec3f;
}

class complex_polygon;

template <class BaseSkin>
class metaballs : public BaseSkin {
	struct impl;
	boost::scoped_ptr<impl> d;

	void init(const std::string& path);

	// Three coordinates: x, y and size
	void render_metaballs(const complex_polygon&, const std::vector<ymse::vec3f>&);

public:
	metaballs(const std::string& path);
	~metaballs();

	void load_opengl_resources();

	void blood(ymse::vec2f p, float r);

	void floor(const complex_polygon&);
};

#endif // METABALLS_HPP
