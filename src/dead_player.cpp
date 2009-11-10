#include "snake.hpp"
#include "dead_player.hpp"

struct dead_player::impl {
	boost::scoped_ptr<snake> s;
	float speed;
};

dead_player::dead_player(std::auto_ptr<snake> s) :
	d(new impl)
{
	d->s.reset(s.release());
	d->speed = 0.5f;
}

dead_player::~dead_player() {
}

void dead_player::move() {
	d->s->forward(d->speed);
}

void dead_player::render(skin& sk) const {
	d->s->render(sk);
}
