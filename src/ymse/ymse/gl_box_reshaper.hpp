#ifndef YMSE_GL_BOX_RESHAPER
#define YMSE_GL_BOX_RESHAPER

#include "reshaper.hpp"

namespace game {
class pixels_per_unit_listener;
}

namespace ymse {

class gl_box_reshaper : public game::reshaper {
	double width, height;
	double x1, y1;
	double x2, y2;

	game::pixels_per_unit_listener* ppu_listener;

	void update_listener();
	void set_projection_matrix();

public:
	gl_box_reshaper();
	~gl_box_reshaper();

	void reshape(int width, int height);

	void set_box(double x1, double y1, double x2, double y2);

	void set_pixels_per_unit_listener(game::pixels_per_unit_listener*);
};

}

#endif

