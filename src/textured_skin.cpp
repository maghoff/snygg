#include <ymse/gl.h>
#include <algorithm>
#include <cmath>
#include <boost/scoped_ptr.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include <ymse/gl/program.hpp>
#include <ymse/gl/shader.hpp>
#include <ymse/gl/texture.hpp>
#include <ymse/sdl/img_load.hpp>
#include <ymse/sdl/surface.hpp>
#include "complex_polygon.hpp"
#include "textured_skin.hpp"
#include "shader_program.hpp"
#include "shader_builder.hpp"

const int across = 5, along = 6, circle_coord = 7, b_attribute = 8;

enum shader_state_t {
	no_shader,
	snakeskin_shader,
	wall_shader,
	food_shader,
	floor_shader
};

struct textured_skin::impl {
	shader_state_t shader_state;

	std::string path;
	
	ymse::sdl::surface diffuse_surface, normal_surface;
	
	boost::scoped_ptr<shader_program> texture_prog;
	boost::scoped_ptr<ymse::gl::program> color_prog, floor_prog;
	ymse::gl::texture diffuse, normal;

	skin::state_t stored_state;
};

textured_skin::textured_skin(const std::string& path) :
	d(new impl)
{
	assert(glGetError() == GL_NO_ERROR);

	d->path = path; //< This is the wrong thing to store.


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

	
	d->shader_state = no_shader;

	d->diffuse_surface = ymse::sdl::img_load(path + "/diffuse.jpg");
	d->normal_surface = ymse::sdl::img_load(path + "/normal.jpg");

	d->stored_state = other_state;
}

textured_skin::~textured_skin() {
}

void textured_skin::load_opengl_resources(int, int) {
	glGetError();
	d->color_prog.reset();
	d->floor_prog.reset();
	glGetError();
	d->color_prog.reset(new ymse::gl::program);
	d->floor_prog.reset(new ymse::gl::program);
	glGetError();

	d->texture_prog->recreate_opengl_resources();

	{
		ymse::gl::shader vertex(GL_VERTEX_SHADER);
		ymse::gl::shader fragment(GL_FRAGMENT_SHADER), light(GL_FRAGMENT_SHADER), mapping(GL_FRAGMENT_SHADER);

		vertex.source_file(d->path + "/vertex.glsl");
		fragment.source_file(d->path + "/fragment.glsl");
		light.source_file(d->path + "/light.glsl");
		mapping.source_file(d->path + "/color_mapping.glsl");

		d->color_prog->attach(vertex);
		d->color_prog->attach(fragment);
		d->color_prog->attach(light);
		d->color_prog->attach(mapping);

		d->color_prog->bind_attrib_location(circle_coord, "circle_coord_in");
		d->color_prog->bind_attrib_location(across, "across_in");
		d->color_prog->bind_attrib_location(along, "along_in");
		d->color_prog->bind_attrib_location(b_attribute, "b_in");

		d->color_prog->link();
	}

	{
		ymse::gl::shader vertex(GL_VERTEX_SHADER);
		ymse::gl::shader fragment(GL_FRAGMENT_SHADER), light(GL_FRAGMENT_SHADER);

		vertex.source_file(d->path + "/mb_vertex.glsl");
		fragment.source_file(d->path + "/flat_fragment.glsl");
		light.source_file(d->path + "/light.glsl");

		d->floor_prog->attach(vertex);
		d->floor_prog->attach(fragment);
		d->floor_prog->attach(light);

		d->floor_prog->link();
	}

	// shaders go out of scope, but are kept alive by OpenGL because of d->prog
	
	d->diffuse_surface.copy_to(d->diffuse);
	d->normal_surface.copy_to(d->normal);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d->diffuse.get_id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, d->normal.get_id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void textured_skin::to_no_shader() {
	if (d->shader_state == no_shader) return;

	glUseProgram(0);

	d->shader_state = no_shader;
}

void textured_skin::to_texture_shader() {
	if (d->stored_state == board_state) to_wall_shader();
	else to_snakeskin_shader();
}

void textured_skin::to_snakeskin_shader() {
	if (d->shader_state == snakeskin_shader) return;

	glUseProgram(d->texture_prog->get_program_id());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d->diffuse.get_id());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, d->normal.get_id());
	d->texture_prog->set_uniform("diffuse_map", 0);
	d->texture_prog->set_uniform("normal_map", 1);
	d->texture_prog->set_uniform("ambient", 0.4f, 0.4f, 0.4f, 1.0f);

	d->shader_state = snakeskin_shader;
}

void textured_skin::to_wall_shader() {
	if (d->shader_state == wall_shader) return;

	glUseProgram(d->color_prog->get_id());

	d->color_prog->set_uniform("ambient", 0.4f, 0.4f, 0.4f, 1.0f);
	d->color_prog->set_uniform("color", 0.1f, 0.1f, 0.1f, 1.0f);

	d->shader_state = wall_shader;
}

void textured_skin::to_food_shader() {
	if (d->shader_state == food_shader) return;

	glUseProgram(d->color_prog->get_id());

	d->color_prog->set_uniform("ambient", 0.4f, 0.4f, 0.4f, 1.0f);
	d->color_prog->set_uniform("color", 0.6f, 0.4f, 0.2f, 1.0f);

	d->shader_state = food_shader;
}

void textured_skin::to_floor_shader() {
	if (d->shader_state == floor_shader) return;

	glUseProgram(d->floor_prog->get_id());

	d->floor_prog->set_uniform("ambient", 0.4f, 0.4f, 0.4f, 1.0f);
	d->floor_prog->set_uniform("diffuse", 0.5f, 0.5f, 0.5f, 1.0f);

	d->shader_state = floor_shader;
}

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
	glUseProgram(0);
	d->shader_state = no_shader;
	to_floor_shader();
	floor_poly.draw();

	to_no_shader();
}

void textured_skin::enter_state(state_t st) {
	d->stored_state = st;
}
