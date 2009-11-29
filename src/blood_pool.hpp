#ifndef SNYGG_BLOOD_POOL_HPP
#define SNYGG_BLOOD_POOL_HPP

#include "segment.hpp"

class blood_pool : public segment, public renderable {
	float x, y, r;
public:
	blood_pool(ymse::vec2f pos, float r);
	~blood_pool();

	void head_forward(float);
	float tail_forward(float);

	bool intersect_with_circle(const ymse::vec2f&, float r) const;

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_tail_pos() const;
	ymse::vec2f get_head_direction() const;

	float length() const;
	void render(skin&, float head_b) const;

	void render(skin&) const;
};


#endif
