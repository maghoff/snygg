#ifndef METABALLS_HPP
#define METABALLS_HPP

#include <boost/scoped_ptr.hpp>
#include <vector>
#include <ymse/vec_fwd.hpp>
#include "scalable_skin.hpp"

class complex_polygon;

class metaballs : public scalable_skin {
	struct impl;
	boost::scoped_ptr<impl> d;

	void init(const std::string& path);

	// Four coordinates: x, y, size and add/remove
	void update_metaballs(const complex_polygon&, const std::vector<ymse::vec4f>&);
	void draw_metaballs(const complex_polygon&);

public:
	metaballs(scalable_skin*, const std::string& path);
	~metaballs();

	void load_opengl_resources(int width, int height);

	void blood(ymse::vec2f p, float r);

	void floor(const complex_polygon&);


	void circle(ymse::vec2f p, float r);
	void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end);
	void cap(ymse::vec2f p, float snake_direction, float cap_direction, float b_coord);

	void set_pixels_per_unit(float);

	void enter_state(state_t);
};

#endif // METABALLS_HPP
