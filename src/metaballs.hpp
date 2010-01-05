#ifndef METABALLS_HPP
#define METABALLS_HPP

#include "textured_skin.hpp"

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
