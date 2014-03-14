#ifndef RECORDING_SKIN_HPP
#define RECORDING_SKIN_HPP

#include <stdexcept>
#include <vector>
#include <scalable_skin.hpp>
#include "geometry_spec.hpp"

class recording_skin : public scalable_skin {
public:
	class unable_to_record : public std::logic_error {
	public:
		using std::logic_error::logic_error;
	};

	std::vector<geometry_spec> recording;

	recording_skin();
	~recording_skin() override;

	void load_opengl_resources(int width, int height) override;

	void set_transformation(const la::matrix33f&) override;

	void circle(la::vec2f p, float r) override;
	void blood(la::vec2f p, float r) override;

	void fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) override;
	void fat_line(la::vec2f p, la::vec2f d, float len, float t, float b_begin, float b_end) override;

	void cap(la::vec2f p, float snake_direction, float cap_direction, float b_coord) override;

	void floor(const complex_polygon&) override;

	void enter_state(state_t) override;
};

#endif // RECORDING_SKIN_HPP
