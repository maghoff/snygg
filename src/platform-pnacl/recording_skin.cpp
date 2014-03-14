#include "recording_skin.hpp"
#include "geometry_spec.hpp"
#include "geometry_builder.hpp"

recording_skin::recording_skin() { }

recording_skin::~recording_skin() { }

void recording_skin::load_opengl_resources(int, int) {
	throw unable_to_record("recording_skin::load_opengl_resources unable to record");
}

void recording_skin::set_transformation(const la::matrix33f& transform) {
	throw unable_to_record("recording_skin::set_transformation unable to record");
}

void recording_skin::circle(la::vec2f p, float r) {
	recording.emplace_back(
		geometry_builder::circle(
			[&](float r) -> float { return get_step_size(r); },
			p, r
		)
	);
}

void recording_skin::blood(la::vec2f p, float r) {
	throw unable_to_record("recording_skin::blood unable to record");
}

void recording_skin::fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
	recording.emplace_back(
		geometry_builder::fat_arc(
			[&](float r) -> float { return get_step_size(r); },
			p, r, t, begin, end, b_begin, b_end
		)
	);
}

void recording_skin::fat_line(la::vec2f p, la::vec2f dir, float len, float t, float b_begin, float b_end) {
	recording.emplace_back(geometry_builder::fat_line(p, dir, len, t, b_begin, b_end));
}

void recording_skin::cap(la::vec2f p, float snake_direction_in, float cap_direction_in, float b_coord) {
	recording.emplace_back(
		geometry_builder::cap(
			[&](float r) -> float { return get_step_size(r); },
			p, snake_direction_in, cap_direction_in, b_coord
		)
	);
}

void recording_skin::floor(const complex_polygon& floor_poly) {
	throw unable_to_record("recording_skin::floor unable to record");
}

void recording_skin::enter_state(state_t st) {
	throw unable_to_record("recording_skin::enter_state unable to record");
}
