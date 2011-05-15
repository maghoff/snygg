#include <ymse/gl.h>
#include <algorithm>
#include <cmath>
#include <boost/scoped_ptr.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "complex_polygon.hpp"
#include "textured_skin.hpp"
#include "shader_program.hpp"
#include "shader_builder.hpp"
#include "shader_configuration.hpp"

const int across = 5, along = 6, circle_coord = 7, b_attribute = 8;

struct textured_skin::impl {
	boost::scoped_ptr<shader_program> texture_prog, color_prog, floor_prog;
	boost::scoped_ptr<shader_configuration> snake_config, wall_config, food_config, floor_config;

	const shader_configuration* shader_state;
	skin::state_t stored_state;
};

textured_skin::textured_skin(const std::string& path) :
	d(new impl)
{
	assert(glGetError() == GL_NO_ERROR);

	{
		shader_builder sb;
		sb.add_shader_from_file(GL_VERTEX_SHADER, path + "/vertex.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/fragment.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/light.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/texture_mapping.glsl");
		sb.bind_attrib_location(circle_coord, "circle_coord_in");
		sb.bind_attrib_location(across, "across_in");
		sb.bind_attrib_location(along, "along_in");
		sb.bind_attrib_location(b_attribute, "b_in");
		d->texture_prog.reset(new shader_program(sb));
	}

	{
		shader_builder sb;
		sb.add_shader_from_file(GL_VERTEX_SHADER, path + "/vertex.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/fragment.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/light.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/color_mapping.glsl");
		sb.bind_attrib_location(circle_coord, "circle_coord_in");
		sb.bind_attrib_location(across, "across_in");
		sb.bind_attrib_location(along, "along_in");
		sb.bind_attrib_location(b_attribute, "b_in");
		d->color_prog.reset(new shader_program(sb));
	}

	{
		shader_builder sb;
		sb.add_shader_from_file(GL_VERTEX_SHADER, path + "/mb_vertex.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/flat_fragment.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/light.glsl");
		d->floor_prog.reset(new shader_program(sb));
	}

	d->snake_config.reset(new shader_configuration(d->texture_prog.get()));
	d->snake_config->set_uniform("ambient", 0.4f, 0.4f, 0.4f, 1.0f);
	d->snake_config->add_texture("diffuse_map", path + "/diffuse.jpg");
	d->snake_config->add_texture("normal_map", path + "/normal.jpg");

	d->wall_config.reset(new shader_configuration(d->color_prog.get()));
	d->wall_config->set_uniform("ambient", 0.4f, 0.4f, 0.4f, 1.0f);
	d->wall_config->set_uniform("color", 0.1f, 0.1f, 0.1f, 1.0f);

	d->food_config.reset(new shader_configuration(d->color_prog.get()));
	d->food_config->set_uniform("ambient", 0.4f, 0.4f, 0.4f, 1.0f);
	d->food_config->set_uniform("color", 0.6f, 0.4f, 0.2f, 1.0f);

	d->floor_config.reset(new shader_configuration(d->floor_prog.get()));
	d->floor_config->set_uniform("ambient", 0.4f, 0.4f, 0.4f, 1.0f);
	d->floor_config->set_uniform("diffuse", 0.5f, 0.5f, 0.5f, 1.0f);

	d->shader_state = 0;
	d->stored_state = other_state;
}

textured_skin::~textured_skin() {
}

void textured_skin::load_opengl_resources(int, int) {
	d->texture_prog->recreate_opengl_resources();
	d->color_prog->recreate_opengl_resources();
	d->floor_prog->recreate_opengl_resources();

	d->snake_config->recreate_opengl_resources();
	d->wall_config->recreate_opengl_resources();
	d->food_config->recreate_opengl_resources();
	d->floor_config->recreate_opengl_resources();
}

void textured_skin::to_shader(const shader_configuration* c) {
	if (d->shader_state == c) return;

	if (c) c->use();
	else glUseProgram(0);

	d->shader_state = c;
}

void textured_skin::to_texture_shader() {
	if (d->stored_state == board_state) to_wall_shader();
	else to_snakeskin_shader();
}

void textured_skin::to_no_shader() { to_shader(0); }
void textured_skin::to_snakeskin_shader() { to_shader(d->snake_config.get()); }
void textured_skin::to_wall_shader() { to_shader(d->wall_config.get()); }
void textured_skin::to_food_shader() { to_shader(d->food_config.get()); }
void textured_skin::to_floor_shader() { to_shader(d->floor_config.get()); }

void textured_skin::circle_core(ymse::vec2f p, float r) {
	float step_size = get_step_size(r);

	glBegin(GL_TRIANGLE_FAN);
	for (float d = 0.f; d < M_PI * 2.f; d += step_size) {
		glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));
	}
	glEnd();
}

void textured_skin::circle(ymse::vec2f p, float r) {
	to_food_shader();

	const float step_size = get_step_size(r);
	const float b_coord = 0;

	float snake_dir = 0;

	glVertexAttrib3f(across, cos(snake_dir), sin(snake_dir), 0);
	glVertexAttrib3f(along, sin(snake_dir), -cos(snake_dir), 0);
	glVertexAttrib1f(b_attribute, b_coord);

	glBegin(GL_TRIANGLE_FAN);

	for (float d = 0; d < 2.0 * M_PI; d += step_size) {
		glVertexAttrib2f(circle_coord, cos(d), sin(d));
		glVertex2f(p[0] + r * cos(snake_dir + d), p[1] + r * sin(snake_dir + d));
	}

	glEnd();
}

void textured_skin::blood(ymse::vec2f p, float r) {
	to_no_shader();

	glColor4f(0.7, 0, 0, 1);
	circle_core(p, r);
	glColor4f(1, 1, 1, 1);
}

void textured_skin::fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
	to_texture_shader();

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

	glBegin(GL_TRIANGLE_STRIP);

	float b = b_begin;

	for (float d = begin; d < end; d += step_size) {
		glVertexAttrib3f(across, direction*cos(d), direction*sin(d), 0);
		glVertexAttrib3f(along, -direction*-sin(d), -direction*cos(d), 0);

		glVertexAttrib2f(circle_coord, inner_a, 0);
		glVertexAttrib1f(b_attribute, b);
		glVertex2f(x + r1 * cos(d), y + r1 * sin(d));
		glVertexAttrib2f(circle_coord, outer_a, 0);
		glVertex2f(x + r2 * cos(d), y + r2 * sin(d));
		b += b_step_size;
	}

	glVertexAttrib3f(across, direction*cos(end), direction*sin(end), 0);
	glVertexAttrib3f(along, -direction*-sin(end), -direction*cos(end), 0);

	glVertexAttrib2f(circle_coord, inner_a, 0);
	glVertexAttrib1f(b_attribute, b_end);
	glVertex2f(x + r1 * cos(end), y + r1 * sin(end));
	glVertexAttrib2f(circle_coord, outer_a, 0);
	glVertex2f(x + r2 * cos(end), y + r2 * sin(end));

	glEnd();
}

void textured_skin::fat_line(ymse::vec2f p, ymse::vec2f dir, float len, float t, float b_begin, float b_end) {
	to_texture_shader();

	float &x1(p[0]), &y1(p[1]), &dx(dir[0]), &dy(dir[1]);

	// Calculate normal * thickness:
	float nx = dy * t, ny = -dx * t;
	float x2 = x1 + dx*len, y2 = y1 + dy * len;

	glBegin(GL_QUADS);
	glVertexAttrib3f(across, nx, ny, 0);
	glVertexAttrib3f(along, -dx, -dy, 0);
	glVertexAttrib2f(circle_coord, 1, 0);
	glVertexAttrib1f(b_attribute, b_begin);
	glVertex2f(x1 + nx, y1 + ny);
	glVertexAttrib3f(across, nx, ny, 0);
	glVertexAttrib3f(along, -dx, -dy, 0);
	glVertexAttrib2f(circle_coord, 1, 0);
	glVertexAttrib1f(b_attribute, b_end);
	glVertex2f(x2 + nx, y2 + ny);
	glVertexAttrib3f(across, nx, ny, 0);
	glVertexAttrib3f(along, -dx, -dy, 0);
	glVertexAttrib2f(circle_coord, -1, 0);
	glVertexAttrib1f(b_attribute, b_end);
	glVertex2f(x2 - nx, y2 - ny);
	glVertexAttrib3f(across, nx, ny, 0);
	glVertexAttrib3f(along, -dx, -dy, 0);
	glVertexAttrib2f(circle_coord, -1, 0);
	glVertexAttrib1f(b_attribute, b_begin);
	glVertex2f(x1 - nx, y1 - ny);
	glEnd();
}

void textured_skin::cap(ymse::vec2f p, float snake_direction_in, float cap_direction_in, float b_coord) {
	to_texture_shader();

	const float r = 2.5;
	const float step_size = get_step_size(r);

	float snake_dir = snake_direction_in - M_PI*0.5;
	float cap_dir = cap_direction_in - M_PI*0.5;

	glVertexAttrib3f(across, cos(snake_dir), sin(snake_dir), 0);
	glVertexAttrib3f(along, sin(snake_dir), -cos(snake_dir), 0);
	glVertexAttrib1f(b_attribute, b_coord);

	glBegin(GL_TRIANGLE_FAN);

	for (float d = cap_dir; d < cap_dir + M_PI; d += step_size) {
		glVertexAttrib2f(circle_coord, cos(d), sin(d));
		glVertex2f(p[0] + r * cos(snake_dir + d), p[1] + r * sin(snake_dir + d));
	}

	float d = cap_dir + M_PI;
	glVertexAttrib2f(circle_coord, cos(d), sin(d));
	glVertex2f(p[0] + r * cos(snake_dir + d), p[1] + r * sin(snake_dir + d));

	glEnd();
}

void textured_skin::floor(const complex_polygon& floor_poly) {
	d->shader_state = 0;
	to_floor_shader();
	floor_poly.draw();

	to_no_shader();
}

void textured_skin::enter_state(state_t st) {
	d->stored_state = st;
}
