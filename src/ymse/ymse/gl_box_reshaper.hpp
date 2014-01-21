#ifndef YMSE_GL_BOX_RESHAPER
#define YMSE_GL_BOX_RESHAPER

#include "reshaper.hpp"

namespace ymse {

class pixels_per_unit_listener;

class gl_box_reshaper : public reshaper {
	double width, height;
	double x1, y1;
	double x2, y2;

	pixels_per_unit_listener* ppu_listener;

	void update_listener();
	void set_projection_matrix();

public:
	gl_box_reshaper();
	~gl_box_reshaper();

	void reshape(int width, int height);

	void set_box(double x1, double y1, double x2, double y2);

	void set_pixels_per_unit_listener(pixels_per_unit_listener*);
};

}

#endif

