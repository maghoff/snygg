#include "snake.hpp"
#include "snake_direction_listener.hpp"

snake_direction_listener::snake_direction_listener() : s(0) { }
snake_direction_listener::~snake_direction_listener() { }

void snake_direction_listener::set_target(snake* s_) {
	s = s_;
}

void snake_direction_listener::set_val(int v) {
	if (s) s->set_turn(v);
}
