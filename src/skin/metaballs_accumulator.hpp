#ifndef METABALLS_ACCUMULATOR_HPP
#define METABALLS_ACCUMULATOR_HPP

#include <set>
#include <vector>
#include <vec.hpp>

class metaballs_accumulator {
	std::multiset<la::vec3f> gen[2];
	int next_gen_index = 0;

	std::multiset<la::vec3f>& prev_gen();
	const std::multiset<la::vec3f>& prev_gen() const;

	std::multiset<la::vec3f>& next_gen();
	const std::multiset<la::vec3f>& next_gen() const;

public:
	metaballs_accumulator();

	void add_to_generation(const la::vec2f& p, float r);
	std::vector<la::vec4f> calculate_updates() const;
	void step_generation();

	void clear();
};

#endif
