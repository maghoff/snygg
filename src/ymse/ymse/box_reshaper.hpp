#ifndef BOX_RESHAPER_HPP
#define BOX_RESHAPER_HPP

#include "reshaper.hpp"
#include "matrix.hpp"

namespace ymse {

class box_reshaper : public reshaper {
	double width, height;
	double x1, y1;
	double x2, y2;

	double ppu;
	matrix33f t, t_inv;

	void calculate_transformation();

public:
	box_reshaper();
	~box_reshaper();

	void reshape(int width, int height);
	void set_box(double x1, double y1, double x2, double y2);

	double get_pixels_per_unit() const;
	const matrix33f& get_transformation() const;
	const matrix33f& get_inverse_transformation() const;
};

} // namespace ymse

#endif // BOX_RESHAPER_HPP
