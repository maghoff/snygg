#include <ymse/vec.hpp>
#include "skin.hpp"
#include "../player.hpp"
#include "food_generator.hpp"
#include "food_item.hpp"

using ymse::vec2f;

struct food_item::impl {
	food_generator& fg;
	vec2f p;
	float r;

	impl(food_generator& fg_) : fg(fg_) { }
};

food_item::food_item(float x, float y, float r, food_generator& fg) :
	d(new impl(fg))
{
	d->p[0] = x;
	d->p[1] = y;
	d->r = r;
}

food_item::~food_item() {
}

void food_item::move() {
	// Stay still!
}

void food_item::hit_by(player& p) {
	p.score();
	d->fg.generate();
	die();
}

void food_item::render(skin& s) const {
	s.circle(d->p, d->r);
}

bool food_item::intersect_with_circle(const ymse::vec2f& p, float r) const {
	vec2f dist(p);
	dist -= d->p;

	float total_r = r + d->r;
	float sq_r = total_r * total_r;

	return dist.square_length() < sq_r;
}
