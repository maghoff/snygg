#ifndef SNAKE_DIRECTION_LISTENER_HPP
#define SNAKE_DIRECTION_LISTENER_HPP

#include <opposite_keys_listener.hpp>

class snake;

class snake_direction_listener : public game::opposite_keys_listener {
	snake* s;

public:
	snake_direction_listener();
	~snake_direction_listener();

	void set_target(snake*);

	void set_val(int);
};

#endif // SNAKE_DIRECTION_LISTENER_HPP
