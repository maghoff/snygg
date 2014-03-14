#include "geometry_builder.hpp"
#include <vector>
#include <cmath>
#include "geometry_spec.hpp"
#include "geometry_mode.hpp"
#include "vertex_spec.hpp"

static const auto fcos = (float (*)(float))(std::cos);
static const auto fsin = (float (*)(float))(std::sin);

static la::vec2f cossin(float a) {
	return la::vec2f(fcos(a), fsin(a));
}

namespace geometry_builder {

geometry_spec circle(const std::function<float(float)>& get_step_size, la::vec2f p, float r) {
	const float step_size = get_step_size(r);
	const float b_coord = 0;

	float snake_dir = 0;

	std::vector<vertex_spec> data;

	for (float d = 0; d < 2.0 * M_PI; d += step_size) {
		data.emplace_back(vertex_spec{
			p + r * cossin(snake_dir + d),
			{ fcos(snake_dir),  fsin(snake_dir), 0 },
			{ fsin(snake_dir), -fcos(snake_dir), 0 },
			cossin(d),
			b_coord
		});
	}

	return { geometry_mode::triangle_fan, data };
}

geometry_spec fat_arc(const std::function<float(float)>& get_step_size, la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
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

	std::vector<vertex_spec> data;

	for (float d = begin; d < end; d += step_size) {
		data.emplace_back(vertex_spec{
			p + r1 * cossin(d),
			{  direction *  fcos(d),  direction * fsin(d), 0},
			{ -direction * -fsin(d), -direction * fcos(d), 0},
			{ inner_a, 0 },
			b
		});

		data.emplace_back(vertex_spec{
			p + r2 * cossin(d),
			{  direction *  fcos(d),  direction * fsin(d), 0},
			{ -direction * -fsin(d), -direction * fcos(d), 0},
			{ outer_a, 0 },
			b
		});

		b += b_step_size;
	}

	data.emplace_back(vertex_spec{
		p + r1 * cossin(end),
		{  direction *  fcos(end),  direction * fsin(end), 0},
		{ -direction * -fsin(end), -direction * fcos(end), 0},
		{ inner_a, 0 },
		b_end
	});

	data.emplace_back(vertex_spec{
		p + r2 * cossin(end),
		{  direction *  fcos(end),  direction * fsin(end), 0},
		{ -direction * -fsin(end), -direction * fcos(end), 0},
		{ outer_a, 0 },
		b_end
	});

	return { geometry_mode::triangle_strip, data };
}

geometry_spec fat_line(la::vec2f p, la::vec2f dir, float len, float t, float b_begin, float b_end) {
	float &x1(p[0]), &y1(p[1]), &dx(dir[0]), &dy(dir[1]);

	// Calculate normal * thickness:
	float nx = dy * t, ny = -dx * t;
	float x2 = x1 + dx*len, y2 = y1 + dy * len;

	std::vector<vertex_spec> data{{
		{ {x1 + nx, y1 + ny}, {nx, ny, 0}, {-dx, -dy, 0}, { 1, 0}, b_begin },
		{ {x1 - nx, y1 - ny}, {nx, ny, 0}, {-dx, -dy, 0}, {-1, 0}, b_begin },
		{ {x2 + nx, y2 + ny}, {nx, ny, 0}, {-dx, -dy, 0}, { 1, 0}, b_end },
		{ {x2 - nx, y2 - ny}, {nx, ny, 0}, {-dx, -dy, 0}, {-1, 0}, b_end },
	}};

	return { geometry_mode::triangle_strip, data };
}

geometry_spec cap(const std::function<float(float)>& get_step_size, la::vec2f p, float snake_direction_in, float cap_direction_in, float b_coord) {
	const float r = 2.5;
	const float step_size = get_step_size(r);

	float snake_dir = snake_direction_in - M_PI*0.5;
	float cap_dir = cap_direction_in - M_PI*0.5;

	la::vec3f across(fcos(snake_dir), fsin(snake_dir), 0);
	la::vec3f along(fsin(snake_dir), -fcos(snake_dir), 0);

	std::vector<vertex_spec> data;

	for (float d = cap_dir; d < cap_dir + M_PI; d += step_size) {
		data.emplace_back(vertex_spec{
			p + r * cossin(snake_dir + d),
			across,
			along,
			cossin(d),
			b_coord
		});
	}

	float d = cap_dir + M_PI;
	data.emplace_back(vertex_spec{
		p + r * cossin(snake_dir + d),
		across,
		along,
		cossin(d),
		b_coord
	});

	return { geometry_mode::triangle_fan, data };
}

}
