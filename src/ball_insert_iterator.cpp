#include <ymse/vec.hpp>
#include "ball_insert_iterator.hpp"

ball_insert_iterator::ball_insert_iterator(std::vector<ymse::vec4f>& container_, float value_) :
	container(container_),
	value(value_)
{
}

ball_insert_iterator& ball_insert_iterator::operator = (const ymse::vec3f& v) {
	container.push_back(ymse::vec4f(v[0], v[1], v[2], value));
	return *this;
}
