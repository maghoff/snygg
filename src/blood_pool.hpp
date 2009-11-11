#ifndef SNYGG_BLOOD_POOL_HPP
#define SNYGG_BLOOD_POOL_HPP

#include "segment.hpp"

class blood_pool : public segment {
	float x, y, r;
public:
	blood_pool(ymse::vec2f pos, float r);
	~blood_pool();

	void head_forward(float);
	float tail_forward(float);

	bool intersect_with_circle(float x, float y, float r) const;

	ymse::vec2f get_head_pos() const;
	ymse::vec2f get_tail_pos() const;
	ymse::vec2f get_head_direction() const;

	float get_radius() const;

	void render(skin&) const;
};


#endif
