#include "player.hpp"
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
	if (d->s->is_single_segment()) die();
}

void dead_player::render(skin& sk) const {
	d->s->render(sk);
}

void dead_player::hit_by(player& p) {
	p.die();
}

bool dead_player::intersect_with_circle(float x, float y, float r) const {
	return d->s->intersect_with_circle(x, y, r);
}
