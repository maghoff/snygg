#include <ymse/vec.hpp>
#include "arc.hpp"
#include "line.hpp"
#include "skin.hpp"
#include "snake.hpp"

using ymse::vec2f;

snake::snake() {
	vec2f pos(0, -40), dir(0, 1);

	body.push_back(new line(pos, dir, 5.f));

	
}

snake::~snake() {
}

void snake::render(skin& s) const {
	vec2f head(body.back().get_head_pos());
	vec2f tail(body.front().get_tail_pos());

	s.circle(head, 2.5f);
	render_sequence(body, s);
	s.circle(tail, 2.5f);
}
