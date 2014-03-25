#ifndef FOOD_ITEM_HPP
#define FOOD_ITEM_HPP

#include <memory>
#include "crashable_with_life.hpp"
#include "renderable.hpp"

class food_generator;

class food_item : public crashable_with_life {
	struct impl;
	std::unique_ptr<impl> d;

public:
	food_item(float x, float y, float r, food_generator&);
	~food_item() override;

	void hit_by(player&) override;
	void render(skin&) const override;
	bool intersect_with_circle(const la::vec2f&, float r) const override;
};

#endif // FOOD_ITEM_HPP
