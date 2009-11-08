#ifndef ITEM_WITH_LIFE_HPP
#define ITEM_WITH_LIFE_HPP

#include "item.hpp"

class item_with_life : public item {
	bool alive;

protected:
	void die();

public:
	item_with_life();
	~item_with_life();

	bool is_dead() const;
};

#endif // ITEM_WITH_LIFE_HPP
