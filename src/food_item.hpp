#ifndef FOOD_ITEM_HPP
#define FOOD_ITEM_HPP

#include <boost/scoped_ptr.hpp>
#include "item_with_life.hpp"

class food_item : public item_with_life {
	struct impl;
	boost::scoped_ptr<impl> d;

public:
	food_item(float x, float y, float r);
	~food_item();

	void hit_by(player&);
	void render(skin&) const;
	bool intersect_with_circle(float x, float y, float r) const;
};

#endif // FOOD_ITEM_HPP
