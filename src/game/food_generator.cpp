#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "board.hpp"
#include "item_container.hpp"
#include "food_item.hpp"
#include "food_generator.hpp"

#include "../config.hpp"

struct food_generator::impl {
	item_container& ic;
	board& b;

	boost::mt19937 rng;
	boost::variate_generator<boost::mt19937&, boost::uniform_real<> > rand;

	impl(item_container& ic_, board& b_) :
		ic(ic_),
		b(b_),
		rand(rng, boost::uniform_real<>(0, 1))
	{ }
};


namespace {

template <class Rand>
ymse::vec2f get_random_pos(Rand rand, ymse::rectf bb) {
	return ymse::vec2f(
		bb.left() + rand() * bb.width(),
		bb.top() + rand() * bb.height()
	);
}

}


food_generator::food_generator(item_container& ic, board& b) :
	d(new impl(ic, b))
{
	d->rng.seed(static_cast<unsigned int>(std::time(0)));
}

food_generator::~food_generator() {
}

void food_generator::generate() {
#ifdef DISALLOW_LONG_PLAYING
	static int count = 0;
	if (count++ >= 3) exit(0);
#endif

	ymse::rectf bb = d->b.bounding_box();

	ymse::vec2f pos;
	do {
		pos = get_random_pos(d->rand, bb);
	} while (
		((d->b.winding_number(pos) & 1) == 0) ||
		(d->b.intersect_with_circle(pos, 5.f))
	);

	d->ic.add_item(std::unique_ptr<item>(
		new food_item(pos[0], pos[1], 5.f, *this)
	));
}
