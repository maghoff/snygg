#ifndef SNYGG_CRASHABLE_WITH_LIFE_HPP
#define SNYGG_CRASHABLE_WITH_LIFE_HPP

#include "crashable.hpp"

class crashable_with_life : virtual public crashable {
	bool alive = true;

protected:
	void die();

public:
	crashable_with_life();
	~crashable_with_life() override;

	bool is_dead() const override;
};

#endif
