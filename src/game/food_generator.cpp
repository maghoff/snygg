#include <ctime>
#include <random>
#include <rect.hpp>
#include <vec.hpp>
#include "board.hpp"
#include "item_container.hpp"
#include "food_item.hpp"
#include "food_generator.hpp"

#include "../config.hpp"

struct food_generator::impl {
	item_container& ic;
	board& b;

	std::mt19937 engine;
	std::uniform_real_distribution<float> distribution;

	impl(item_container& ic_, board& b_) :
		ic(ic_),
		b(b_),
		distribution(0, 1)
	{ }

	float rand() {
		return distribution(engine);
	}
};


namespace {

template <class Rand>
la::vec2f get_random_pos(Rand rand, rectf bb) {
	return la::vec2f(
		bb.left() + rand() * bb.width(),
		bb.top() + rand() * bb.height()
	);
}

}


food_generator::food_generator(item_container& ic, board& b) :
	d(new impl(ic, b))
{
	d->engine.seed(static_cast<unsigned int>(std::time(0)));
}

food_generator::~food_generator() {
}

void food_generator::generate() {
#ifdef DISALLOW_LONG_PLAYING
	static int count = 0;
	if (count++ >= 3) exit(0);
#endif

	rectf bb = d->b.bounding_box();

	la::vec2f pos;
	do {
		pos = get_random_pos([&]{ return d->rand(); }, bb);
	} while (
		((d->b.winding_number(pos) & 1) == 0) ||
		(d->b.intersect_with_circle(pos, 5.f))
	);

	d->ic.add_item(std::unique_ptr<item>(
		new food_item(pos[0], pos[1], 5.f, *this)
	));
}
