#ifndef BOX_RESHAPER_HPP
#define BOX_RESHAPER_HPP

#include <matrix.hpp>
#include "reshaper.hpp"

namespace game {

class box_reshaper : public reshaper {
	double width, height;
	double x1, y1;
	double x2, y2;

	double ppu;
	la::matrix33f t, t_inv;

	void calculate_transformation();

public:
	box_reshaper();
	~box_reshaper();

	void reshape(int width, int height);
	void set_box(double x1, double y1, double x2, double y2);

	double get_pixels_per_unit() const;
	const la::matrix33f& get_transformation() const;
	const la::matrix33f& get_inverse_transformation() const;
};

} // namespace game

#endif // BOX_RESHAPER_HPP
