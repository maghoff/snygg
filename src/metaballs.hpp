#ifndef METABALLS_HPP
#define METABALLS_HPP

#include <boost/scoped_ptr.hpp>
#include <vector>
#include "scalable_skin.hpp"

namespace ymse {
	template <int Dim, typename T>
	class vec;

	typedef vec<2, float> vec2f;
	typedef vec<4, float> vec4f;
}

class complex_polygon;

class metaballs : public scalable_skin {
	struct impl;
	boost::scoped_ptr<impl> d;

	void init(const std::string& path);

	// Three coordinates: x, y and size
	void render_metaballs(const complex_polygon&, const std::vector<ymse::vec4f>&);

public:
	metaballs(scalable_skin*, const std::string& path);
	~metaballs();

	void load_opengl_resources();

	void blood(ymse::vec2f p, float r);

	void floor(const complex_polygon&);


	void circle(ymse::vec2f p, float r);
	void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end);
	void cap(ymse::vec2f p, float snake_direction, float cap_direction, float b_coord);

	void set_pixels_per_unit(float);
};

#endif // METABALLS_HPP
