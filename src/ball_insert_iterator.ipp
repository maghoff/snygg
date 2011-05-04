#ifndef BALL_INSERT_ITERATOR_IPP
#define BALL_INSERT_ITERATOR_IPP

#include <iterator>
#include <vector>
#include <ymse/vec.hpp>

namespace {

class ball_insert_iterator :
	public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	std::vector<ymse::vec4f>& container;
	float value;

public:
	ball_insert_iterator(std::vector<ymse::vec4f>&, float);
	ball_insert_iterator& operator = (const ymse::vec3f& v);

	ball_insert_iterator& operator*() { return *this; }
	ball_insert_iterator& operator++() { return *this; }
	ball_insert_iterator& operator++(int) { return *this; }
};

ball_insert_iterator::ball_insert_iterator(std::vector<ymse::vec4f>& container_, float value_) :
	container(container_),
	value(value_)
{
}

ball_insert_iterator& ball_insert_iterator::operator = (const ymse::vec3f& v) {
	container.push_back(ymse::vec4f(v[0], v[1], v[2], value));
	return *this;
}

}

#endif // BALL_INSERT_ITERATOR_IPP
