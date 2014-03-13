#include "buffering_skin.hpp"
#include <GLES2/gl2.h>
#include "geometry_spec.hpp"
#include "geometry_builder.hpp"

namespace attrib {
	enum {
		vertex = 0,
		circle_coord,
		across,
		along,
		b
	};
}

buffering_skin::buffering_skin() : buffer(0) {
}

buffering_skin::~buffering_skin() {
	if (buffer != 0) glDeleteBuffers(1, &buffer);
}

void buffering_skin::drawGeometrySpec(const geometry_spec& spec) {
	if (buffer == 0) glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, spec.data.size() * sizeof(spec.data[0]), spec.data.data(), GL_STREAM_DRAW);

	glEnableVertexAttribArray(attrib::vertex);
	glEnableVertexAttribArray(attrib::across);
	glEnableVertexAttribArray(attrib::along);
	glEnableVertexAttribArray(attrib::circle_coord);
	glEnableVertexAttribArray(attrib::b);

	auto stride = sizeof(spec.data[0]);
	glVertexAttribPointer(attrib::vertex, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, vertex));
	glVertexAttribPointer(attrib::across, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, across));
	glVertexAttribPointer(attrib::along, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, along));
	glVertexAttribPointer(attrib::circle_coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, circle_coord));
	glVertexAttribPointer(attrib::b, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertex_spec, b));

	glDrawArrays(spec.mode == geometry_spec::triangle_fan ? GL_TRIANGLE_FAN : GL_TRIANGLE_STRIP, 0, spec.data.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void buffering_skin::load_opengl_resources(int, int) {
}

void buffering_skin::set_transformation(const la::matrix33f& transform) {
}

void buffering_skin::circle(la::vec2f p, float r) {
	drawGeometrySpec(
		geometry_builder::circle(
			[&](float r) -> float { return get_step_size(r); },
			p, r
		)
	);
}

void buffering_skin::blood(la::vec2f p, float r) {
}

void buffering_skin::fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
	drawGeometrySpec(
		geometry_builder::fat_arc(
			[&](float r) -> float { return get_step_size(r); },
			p, r, t, begin, end, b_begin, b_end
		)
	);
}

void buffering_skin::fat_line(la::vec2f p, la::vec2f dir, float len, float t, float b_begin, float b_end) {
	drawGeometrySpec(geometry_builder::fat_line(p, dir, len, t, b_begin, b_end));
}

void buffering_skin::cap(la::vec2f p, float snake_direction_in, float cap_direction_in, float b_coord) {
	drawGeometrySpec(
		geometry_builder::cap(
			[&](float r) -> float { return get_step_size(r); },
			p, snake_direction_in, cap_direction_in, b_coord
		)
	);
}

void buffering_skin::floor(const complex_polygon& floor_poly) {
}

void buffering_skin::enter_state(state_t st) {
}
