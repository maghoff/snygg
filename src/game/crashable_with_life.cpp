#include "crashable_with_life.hpp"

crashable_with_life::crashable_with_life() { }
crashable_with_life::~crashable_with_life() { }

void crashable_with_life::die() {
	alive = false;
}

bool crashable_with_life::is_dead() const {
	return !alive;
}
