#include <ymse/vec.hpp>
#include "blood_pool.hpp"
#include "item_container.hpp"
#include "player.hpp"
#include "snake.hpp"
#include "dead_player.hpp"

struct dead_player::impl {
	item_container& ic;
	boost::scoped_ptr<snake> s;
	blood_pool* pool;
	float speed;

	impl(item_container& ic_) : ic(ic_) { }
};

dead_player::dead_player(std::auto_ptr<snake> s, item_container& ic_) :
	d(new impl(ic_))
{
	d->s.reset(s.release());
	d->pool = d->s->crack_head();
	d->speed = 0.5f;
}

dead_player::~dead_player() {
}

void dead_player::move() {
	d->s->forward(d->speed);
	if (d->s->is_single_segment()) {
		d->ic.add_renderable(std::auto_ptr<renderable>(new blood_pool(d->pool->get_head_pos(), d->pool->get_radius())));
		die();
	}
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
