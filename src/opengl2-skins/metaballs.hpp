#ifndef METABALLS_HPP
#define METABALLS_HPP

#include <memory>
#include <vector>
#include <vec_fwd.hpp>
#include "scalable_skin.hpp"

struct complex_polygon;

class metaballs : public scalable_skin {
	struct impl;
	std::unique_ptr<impl> d;

	void init(const std::string& path);

	// Four coordinates: x, y, size and add/remove
	void update_metaballs(const complex_polygon&, const std::vector<la::vec4f>&);
	void draw_metaballs(const complex_polygon&);

public:
	metaballs(scalable_skin*, const std::string& path);
	~metaballs();

	void load_opengl_resources(int width, int height);

	void blood(la::vec2f p, float r);

	void floor(const complex_polygon&);


	void circle(la::vec2f p, float r);
	void fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(la::vec2f p, la::vec2f d, float len, float t, float b_begin, float b_end);
	void cap(la::vec2f p, float snake_direction, float cap_direction, float b_coord);

	void set_pixels_per_unit(float);

	void enter_state(state_t);
};

#endif // METABALLS_HPP
