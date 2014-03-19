#include "metaballs_accumulator.hpp"
#include <algorithm>
#include <set>


namespace {

class ball_insert_iterator :
	public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	std::vector<la::vec4f>& container;
	float value;

public:
	ball_insert_iterator(std::vector<la::vec4f>&, float);
	ball_insert_iterator& operator = (const la::vec3f& v);

	ball_insert_iterator& operator*() { return *this; }
	ball_insert_iterator& operator++() { return *this; }
	ball_insert_iterator& operator++(int) { return *this; }
};

ball_insert_iterator::ball_insert_iterator(std::vector<la::vec4f>& container_, float value_) :
	container(container_),
	value(value_)
{
}

ball_insert_iterator& ball_insert_iterator::operator = (const la::vec3f& v) {
	container.push_back(la::vec4f(v[0], v[1], v[2], value));
	return *this;
}

}


metaballs_accumulator::metaballs_accumulator() { }

std::multiset<la::vec3f>& metaballs_accumulator::prev_gen() { return gen[next_gen_index ^ 1]; }
const std::multiset<la::vec3f>& metaballs_accumulator::prev_gen() const { return gen[next_gen_index ^ 1]; }
std::multiset<la::vec3f>& metaballs_accumulator::next_gen() { return gen[next_gen_index]; }
const std::multiset<la::vec3f>& metaballs_accumulator::next_gen() const { return gen[next_gen_index]; }

void metaballs_accumulator::add_to_generation(const la::vec2f& p, float r) {
	next_gen().insert(la::vec3f(p[0], p[1], r));
}

std::vector<la::vec4f> metaballs_accumulator::calculate_updates() const {
	std::vector<la::vec4f> balls;

	// Removed balls:
	std::set_difference(
		prev_gen().begin(), prev_gen().end(),
		next_gen().begin(), next_gen().end(),
		ball_insert_iterator(balls, -1)
	);

	// Added balls:
	std::set_difference(
		next_gen().begin(), next_gen().end(),
		prev_gen().begin(), prev_gen().end(),
		ball_insert_iterator(balls, 1)
	);

	return balls;
}

void metaballs_accumulator::step_generation() {
	next_gen_index ^= 1;
	next_gen().clear();
}

void metaballs_accumulator::clear() {
	gen[0].clear();
	gen[1].clear();
}
