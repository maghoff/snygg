#ifndef FOOD_ITEM_HPP
#define FOOD_ITEM_HPP

#include <boost/scoped_ptr.hpp>
#include "item_with_life.hpp"

class food_generator;

class food_item : public item_with_life {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	food_item(float x, float y, float r, food_generator&);
	~food_item();

	void move();
	void hit_by(player&);
	void render(skin&) const;
	bool intersect_with_circle(const ymse::vec2f&, float r) const;
};

#endif // FOOD_ITEM_HPP
