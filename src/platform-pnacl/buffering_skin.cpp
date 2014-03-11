#include "buffering_skin.hpp"
#include <GLES2/gl2.h>
#include <array>
#include <vector>
#include <cmath>

static const auto fcos = (float (*)(float))(std::cos);
static const auto fsin = (float (*)(float))(std::sin);

la::vec2f cossin(float a) {
	return la::vec2f(fcos(a), fsin(a));
}

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

void buffering_skin::drawVertexSpec(const std::vector<vertexSpec>& data, unsigned mode) {
	if (buffer == 0) glGenBuffers(1, &buffer);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_STREAM_DRAW);

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

	glDrawArrays(mode, 0, data.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void buffering_skin::load_opengl_resources(int, int) {
}

void buffering_skin::set_transformation(const la::matrix33f& transform) {
}

void buffering_skin::circle(la::vec2f p, float r) {
	const float step_size = get_step_size(r);
	const float b_coord = 0;

	float snake_dir = 0;

	std::vector<vertexSpec> data;

	for (float d = 0; d < 2.0 * M_PI; d += step_size) {
		data.emplace_back(vertexSpec{
			p + r * cossin(snake_dir + d),
			{ fcos(snake_dir),  fsin(snake_dir), 0 },
			{ fsin(snake_dir), -fcos(snake_dir), 0 },
			cossin(d),
			b_coord
		});
	}

	drawVertexSpec(data, GL_TRIANGLE_FAN);
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

	float b = b_begin;

	std::vector<vertexSpec> data;

	for (float d = begin; d < end; d += step_size) {
		data.emplace_back(vertexSpec{
			p + r1 * cossin(d),
			{  direction *  fcos(d),  direction * fsin(d), 0},
			{ -direction * -fsin(d), -direction * fcos(d), 0},
			{ inner_a, 0 },
			b
		});

		data.emplace_back(vertexSpec{
			p + r2 * cossin(d),
			{  direction *  fcos(d),  direction * fsin(d), 0},
			{ -direction * -fsin(d), -direction * fcos(d), 0},
			{ outer_a, 0 },
			b
		});

		b += b_step_size;
	}

	data.emplace_back(vertexSpec{
		p + r1 * cossin(end),
		{  direction *  fcos(end),  direction * fsin(end), 0},
		{ -direction * -fsin(end), -direction * fcos(end), 0},
		{ inner_a, 0 },
		b_end
	});

	data.emplace_back(vertexSpec{
		p + r2 * cossin(end),
		{  direction *  fcos(end),  direction * fsin(end), 0},
		{ -direction * -fsin(end), -direction * fcos(end), 0},
		{ outer_a, 0 },
		b_end
	});

	drawVertexSpec(data, GL_TRIANGLE_STRIP);
}

void buffering_skin::fat_line(la::vec2f p, la::vec2f dir, float len, float t, float b_begin, float b_end) {
	float &x1(p[0]), &y1(p[1]), &dx(dir[0]), &dy(dir[1]);

	// Calculate normal * thickness:
	float nx = dy * t, ny = -dx * t;
	float x2 = x1 + dx*len, y2 = y1 + dy * len;

	std::vector<vertexSpec> data{{
		{ {x1 + nx, y1 + ny}, {nx, ny, 0}, {-dx, -dy, 0}, { 1, 0}, b_begin },
		{ {x1 - nx, y1 - ny}, {nx, ny, 0}, {-dx, -dy, 0}, {-1, 0}, b_begin },
		{ {x2 + nx, y2 + ny}, {nx, ny, 0}, {-dx, -dy, 0}, { 1, 0}, b_end },
		{ {x2 - nx, y2 - ny}, {nx, ny, 0}, {-dx, -dy, 0}, {-1, 0}, b_end },
	}};

	drawVertexSpec(data, GL_TRIANGLE_STRIP);
}

void buffering_skin::cap(la::vec2f p, float snake_direction_in, float cap_direction_in, float b_coord) {
	const float r = 2.5;
	const float step_size = get_step_size(r);

	float snake_dir = snake_direction_in - M_PI*0.5;
	float cap_dir = cap_direction_in - M_PI*0.5;

	la::vec3f across(fcos(snake_dir), fsin(snake_dir), 0);
	la::vec3f along(fsin(snake_dir), -fcos(snake_dir), 0);

	std::vector<vertexSpec> data;

	for (float d = cap_dir; d < cap_dir + M_PI; d += step_size) {
		data.emplace_back(vertexSpec{
			p + r * cossin(snake_dir + d),
			across,
			along,
			cossin(d),
			b_coord
		});
	}

	float d = cap_dir + M_PI;
	data.emplace_back(vertexSpec{
		p + r * cossin(snake_dir + d),
		across,
		along,
		cossin(d),
		b_coord
	});

	drawVertexSpec(data, GL_TRIANGLE_FAN);
}

void buffering_skin::floor(const complex_polygon& floor_poly) {
}

void buffering_skin::enter_state(state_t st) {
}
