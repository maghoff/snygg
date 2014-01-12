#include "item_with_life.hpp"

item_with_life::item_with_life() :
	alive(true)
{
}

item_with_life::~item_with_life() {
}

void item_with_life::die() {
	alive = false;
}

bool item_with_life::is_dead() const {
	return !alive;
}
