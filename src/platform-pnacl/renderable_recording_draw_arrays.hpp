#ifndef RENDERABLE_RECORDING_DRAW_ARRAYS_HPP
#define RENDERABLE_RECORDING_DRAW_ARRAYS_HPP

#include <vector>
#include "geometry_mode.hpp"

struct geometry_spec;
class buffering_skin;

class renderable_recording_draw_arrays {
	std::vector<unsigned> buffers;
	std::vector<std::pair<unsigned, geometry_mode>> details;

public:
	void update(const std::vector<geometry_spec>&);

	void render(buffering_skin&);
};

#endif
