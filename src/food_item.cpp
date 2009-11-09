#include <ymse/vec.hpp>
#include "player.hpp"
#include "skin.hpp"
#include "food_item.hpp"

struct food_item::impl {
	ymse::vec2f p;
	float r;
};

food_item::food_item(float x, float y, float r) :
	d(new impl)
{
	d->p[0] = x;
	d->p[1] = y;
	d->r = r;
}

food_item::~food_item() {
}

void food_item::hit_by(player& p) {
	p.score();
	die();
}

void food_item::render(skin& s) const {
	s.circle(d->p, d->r);
}

bool food_item::intersect_with_circle(float x, float y, float r) const {
	float dx = x - d->p[0];
	float dy = y - d->p[1];
	float sq_d = dx*dx + dy*dy;

	float total_r = r + d->r;
	float sq_r = total_r * total_r;

	return sq_d < sq_r;
}
