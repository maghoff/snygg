#include <ctime>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include "item_container.hpp"
#include "food_item.hpp"
#include "food_generator.hpp"

struct food_generator::impl {
	item_container& ic;

	boost::mt19937 rng;
	boost::variate_generator<boost::mt19937&, boost::uniform_real<> > rand;

	impl(item_container& ic_) :
		ic(ic_),
		rand(rng, boost::uniform_real<>(0, 1))
	{ }
};


food_generator::food_generator(item_container& ic, board&) :
	d(new impl(ic))
{
	d->rng.seed(static_cast<unsigned int>(std::time(0)));
}

food_generator::~food_generator() {
}

void food_generator::generate() {
	const float e1 = 7.5f, e2 = e1 * 2.f;
	d->ic.add_item(std::auto_ptr<item>(
		new food_item(
			d->rand() * (400. - e2) - (200. - e1),
			d->rand() * (100. - e2) - (50. - e1),
			5.f,
			*this
		))
	);
}
