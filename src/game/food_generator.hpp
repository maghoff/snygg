#ifndef FOOD_GENERATOR_HPP
#define FOOD_GENERATOR_HPP

#include <memory>

class item_container;
class board;

class food_generator {
	struct impl;
	std::unique_ptr<impl> d;

public:
	food_generator(item_container&, board&);
	~food_generator();

	void generate();
};

#endif // FOOD_GENERATOR_HPP
