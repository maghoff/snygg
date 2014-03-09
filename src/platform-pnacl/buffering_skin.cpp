#include "buffering_skin.hpp"
#include <GLES2/gl2.h>
#include <array>
#include <vector>
#include <cmath>

static const auto fcos = (float (*)(float))(std::cos);
static const auto fsin = (float (*)(float))(std::sin);

namespace attrib {
	enum {
		vertex = 0,
		circle_coord,
		across,
		along,
		b
	};
}

struct vertexSpec {
	la::vec2f vertex;
	la::vec3f across;
	la::vec3f along;
	la::vec2f circle_coord;
	float b;
};

buffering_skin::buffering_skin() : buffer(0) {
}

buffering_skin::~buffering_skin() {
	if (buffer != 0) glDeleteBuffers(1, &buffer);
}

void buffering_skin::drawVertexSpec(const std::vector<vertexSpec>& data) {
	if (buffer == 0) glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(attrib::vertex);
	glEnableVertexAttribArray(attrib::across);
	glEnableVertexAttribArray(attrib::along);
	glEnableVertexAttribArray(attrib::circle_coord);
	glEnableVertexAttribArray(attrib::b);

	auto stride = sizeof(data[0]);
	glVertexAttribPointer(attrib::vertex, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertexSpec, vertex));
	glVertexAttribPointer(attrib::across, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertexSpec, across));
	glVertexAttribPointer(attrib::along, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertexSpec, along));
	glVertexAttribPointer(attrib::circle_coord, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertexSpec, circle_coord));
	glVertexAttribPointer(attrib::b, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(vertexSpec, b));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, data.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void buffering_skin::load_opengl_resources(int, int) {
}

void buffering_skin::set_transformation(const la::matrix33f& transform) {
}

void buffering_skin::circle(la::vec2f p, float r) {
}

void buffering_skin::blood(la::vec2f p, float r) {
}

void buffering_skin::fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
	float r1 = r-t, r2 = r+t;
	float step_size = get_step_size(r2);
	float inner_a = -1, outer_a = 1;
	float direction = 1;
	if (begin > end) {
		std::swap(begin, end);
		std::swap(b_begin, b_end);
		std::swap(inner_a, outer_a);
		direction *= -1;
	}

	float b_step_size = (b_end - b_begin) / ((end - begin) / step_size);

	float &x(p[0]), &y(p[1]);

	float b = b_begin;

	std::vector<vertexSpec> data;

	for (float d = begin; d < end; d += step_size) {
		data.emplace_back(vertexSpec{
			{ x + r1 * fcos(d), y + r1 * fsin(d)},
			{  direction *  fcos(d),  direction * fsin(d), 0},
			{ -direction * -fsin(d), -direction * fcos(d), 0},
			{ inner_a, 0 },
			b
		});

		data.emplace_back(vertexSpec{
			{ x + r2 * fcos(d), y + r2 * fsin(d)},
			{  direction *  fcos(d),  direction * fsin(d), 0},
			{ -direction * -fsin(d), -direction * fcos(d), 0},
			{ outer_a, 0 },
			b
		});

		b += b_step_size;
	}

	data.emplace_back(vertexSpec{
		{ x + r1 * fcos(end), y + r1 * fsin(end)},
		{  direction *  fcos(end),  direction * fsin(end), 0},
		{ -direction * -fsin(end), -direction * fcos(end), 0},
		{ inner_a, 0 },
		b_end
	});

	data.emplace_back(vertexSpec{
		{ x + r2 * fcos(end), y + r2 * fsin(end)},
		{  direction *  fcos(end),  direction * fsin(end), 0},
		{ -direction * -fsin(end), -direction * fcos(end), 0},
		{ outer_a, 0 },
		b_end
	});

	drawVertexSpec(data);
}

void buffering_skin::fat_line(la::vec2f p, la::vec2f dir, float len, float t, float b_begin, float b_end) {
	float &x1(p[0]), &y1(p[1]), &dx(dir[0]), &dy(dir[1]);

	// Calculate normal * thickness:
	float nx = dy * t, ny = -dx * t;
	float x2 = x1 + dx*len, y2 = y1 + dy * len;

	std::vector<vertexSpec> data{{
		/* vertex (x, y), across (x, y, z), along (x, y, z), circle_coord (x, y), b */
		{ {x1 + nx, y1 + ny}, {nx, ny, 0}, {-dx, -dy, 0}, { 1, 0}, b_begin },
		{ {x1 - nx, y1 - ny}, {nx, ny, 0}, {-dx, -dy, 0}, {-1, 0}, b_begin },
		{ {x2 + nx, y2 + ny}, {nx, ny, 0}, {-dx, -dy, 0}, { 1, 0}, b_end },
		{ {x2 - nx, y2 - ny}, {nx, ny, 0}, {-dx, -dy, 0}, {-1, 0}, b_end },
	}};

	drawVertexSpec(data);
}

void buffering_skin::cap(la::vec2f p, float snake_direction_in, float cap_direction_in, float b_coord) {
}

void buffering_skin::floor(const complex_polygon& floor_poly) {
}

void buffering_skin::enter_state(state_t st) {
}
