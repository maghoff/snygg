#ifndef BALL_INSERT_ITERATOR_HPP
#define BALL_INSERT_ITERATOR_HPP

#include <iterator>
#include <vector>
#include <ymse/vec_fwd.hpp>

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
};

#endif // BALL_INSERT_ITERATOR_HPP
