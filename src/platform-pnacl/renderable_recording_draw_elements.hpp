#ifndef RENDERABLE_RECORDING_DRAW_ELEMENTS_HPP
#define RENDERABLE_RECORDING_DRAW_ELEMENTS_HPP

#include <array>
#include <vector>
#include "geometry_mode.hpp"

struct geometry_spec;
class buffering_skin;

class renderable_recording_draw_elements {
	std::array<unsigned, 2> buffers;
	unsigned elements;

public:
	renderable_recording_draw_elements();

	void update(const std::vector<geometry_spec>&);

	void render(buffering_skin&);
};

#endif
