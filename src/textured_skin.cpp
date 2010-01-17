#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include <ymse/gl/program.hpp>
#include <ymse/gl/shader.hpp>
#include <ymse/gl/texture.hpp>
#include <ymse/sdl/img_load.hpp>
#include <ymse/sdl/surface.hpp>
#include "textured_skin.hpp"

const int snake_coord = 4, across = 5, along = 6, circle_coord = 7, b_attribute = 8;

enum shader_state_t {
	no_shader,
	cap_shader
};

struct textured_skin::impl {
	shader_state_t shader_state;

	ymse::gl::program cap;
	ymse::gl::texture diffuse, normal;
};

textured_skin::textured_skin(const std::string& path) :
	d(new impl)
{
	assert(glGetError() == GL_NO_ERROR);

	d->shader_state = no_shader;

	ymse::gl::shader light(GL_FRAGMENT_SHADER);
	light.source_file(path + "/light.glsl");

	// shaders go out of scope, but are kept alive by OpenGL because of d->prog

	ymse::gl::shader cap_vertex(GL_VERTEX_SHADER), cap_fragment(GL_FRAGMENT_SHADER);

	cap_vertex.source_file(path + "/cap_vertex.glsl");
	cap_fragment.source_file(path + "/cap_fragment.glsl");

	d->cap.attach(cap_vertex);
	d->cap.attach(cap_fragment);
	d->cap.attach(light);

	d->cap.bind_attrib_location(circle_coord, "circle_coord_in");
	d->cap.bind_attrib_location(across, "across_in");
	d->cap.bind_attrib_location(along, "along_in");
	d->cap.bind_attrib_location(b_attribute, "b_in");

	d->cap.link();

	ymse::sdl::img_load(path + "/diffuse.jpg").copy_to(d->diffuse);
	ymse::sdl::img_load(path + "/normal.jpg").copy_to(d->normal);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d->diffuse.get_id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, d->normal.get_id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

textured_skin::~textured_skin() {
}

void textured_skin::to_no_shader() {
	if (d->shader_state == no_shader) return;

	glUseProgram(0);

	d->shader_state = no_shader;
}

void textured_skin::to_cap_shader() {
	if (d->shader_state == cap_shader) return;

	glUseProgram(d->cap.get_id());

	d->cap.set_uniform("diffuse_map", 0);
	d->cap.set_uniform("normal_map", 1);

	d->shader_state = cap_shader;
}

void textured_skin::circle(ymse::vec2f p, float r) {
	to_no_shader();

	float step_size = get_step_size(r);

	glBegin(GL_TRIANGLE_FAN);
	for (float d = 0.f; d < M_PI * 2.f; d += step_size) {
		glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));
	}
	glEnd();
}

void textured_skin::blood(ymse::vec2f p, float r) {
	to_no_shader();

	glColor4f(1, 0, 0, 1);
	circle(p, r);
	glColor4f(1, 1, 1, 1);
}

void textured_skin::fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
	to_cap_shader();

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
	to_cap_shader();

	float &x1(p[0]), &y1(p[1]), &dx(dir[0]), &dy(dir[1]);

	// Calculate normal * thickness:
	float nx = dy * t, ny = -dx * t;
	float x2 = x1 + dx*len, y2 = y1 + dy * len;

	glVertexAttrib3f(across, nx, ny, 0);
	glVertexAttrib3f(along, -dx, -dy, 0);

	glBegin(GL_QUADS);
	glVertexAttrib2f(circle_coord, 1, 0);
	glVertexAttrib1f(b_attribute, b_begin);
	glVertex2f(x1 + nx, y1 + ny);
	glVertexAttrib2f(circle_coord, 1, 0);
	glVertexAttrib1f(b_attribute, b_end);
	glVertex2f(x2 + nx, y2 + ny);
	glVertexAttrib2f(circle_coord, -1, 0);
	glVertexAttrib1f(b_attribute, b_end);
	glVertex2f(x2 - nx, y2 - ny);
	glVertexAttrib2f(circle_coord, -1, 0);
	glVertexAttrib1f(b_attribute, b_begin);
	glVertex2f(x1 - nx, y1 - ny);
	glEnd();
}

void textured_skin::cap_front(ymse::vec2f p, float direction, float b_coord) {
	to_cap_shader();

	const float r = 2.5;
	const float step_size = get_step_size(r);

	float base_ang = direction - M_PI*0.5;

	glVertexAttrib3f(across, cos(base_ang), sin(base_ang), 0);
	glVertexAttrib3f(along, sin(base_ang), -cos(base_ang), 0);
	glVertexAttrib1f(b_attribute, b_coord);

	glBegin(GL_TRIANGLE_FAN);
	for (float d = direction - M_PI * 0.5; d < direction + M_PI * 0.5; d += step_size) {
		glVertexAttrib2f(circle_coord, cos(d-base_ang), sin(d-base_ang));
		glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));
	}
	float d = direction + M_PI * 0.5;
	glVertexAttrib2f(circle_coord, cos(d-base_ang), sin(d-base_ang));
	glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));

	glEnd();
}

void textured_skin::cap_back(ymse::vec2f p, float direction, float b_coord) {
	to_cap_shader();

	const float r = 2.5;
	const float step_size = get_step_size(r);

	float base_ang = direction - M_PI*0.5;

	glVertexAttrib3f(across, cos(base_ang), sin(base_ang), 0);
	glVertexAttrib3f(along, sin(base_ang), -cos(base_ang), 0);
	glVertexAttrib1f(b_attribute, b_coord);

	glBegin(GL_TRIANGLE_FAN);
	for (float d = direction - M_PI * 0.5 + M_PI; d < direction + M_PI * 0.5 + M_PI; d += step_size) {
		glVertexAttrib2f(circle_coord, cos(d-base_ang), sin(d-base_ang));
		glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));
	}
	float d = direction + M_PI * 0.5 + M_PI;
	glVertexAttrib2f(circle_coord, cos(d-base_ang), sin(d-base_ang));
	glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));

	glEnd();
}

void textured_skin::finish_frame(ymse::rectf bounding_box) {
	d->shader_state = no_shader; //< Because of metaballs
}
