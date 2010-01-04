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

const int snake_coord = 4, across = 5, along = 6;

struct textured_skin::impl {
	ymse::gl::program prog, metaballs;
	ymse::gl::texture diffuse, normal;
};

textured_skin::textured_skin(const std::string& path) :
	d(new impl)
{
	ymse::gl::shader vertex(GL_VERTEX_SHADER), fragment(GL_FRAGMENT_SHADER);

	vertex.source_file(path + "/vertex.glsl");
	fragment.source_file(path + "/fragment.glsl");

	d->prog.attach(vertex);
	d->prog.attach(fragment);

	d->prog.bind_attrib_location(snake_coord, "snake_coord_in");
	d->prog.bind_attrib_location(across, "across_in");
	d->prog.bind_attrib_location(along, "along_in");

	d->prog.link();

	// shaders go out of scope, but are kept alive by OpenGL because of d->prog

	ymse::sdl::img_load(path + "/diffuse.jpg").copy_to(d->diffuse);
	ymse::sdl::img_load(path + "/normal.jpg").copy_to(d->normal);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d->diffuse.get_id());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, d->normal.get_id());



	ymse::gl::shader mb_vertex(GL_VERTEX_SHADER), mb_fragment(GL_FRAGMENT_SHADER);

	mb_vertex.source_file(path + "/mb_vertex.glsl");
	mb_fragment.source_file(path + "/mb_fragment.glsl");

	d->metaballs.attach(mb_vertex);
	d->metaballs.attach(mb_fragment);

	d->metaballs.link();
}

void textured_skin::circle(ymse::vec2f p, float r) {
	float step_size = get_step_size(r);

	glBegin(GL_TRIANGLE_FAN);
	for (float d = 0.f; d < M_PI * 2.f; d += step_size) {
		glVertex2f(p[0] + r * cos(d), p[1] + r * sin(d));
	}
	glEnd();
}

void textured_skin::fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
	glUseProgram(d->prog.get_id());

	d->prog.set_uniform("diffuse_map", 0);
	d->prog.set_uniform("normal_map", 1);

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

		glVertexAttrib2f(snake_coord, inner_a, b);
		glVertex2f(x + r1 * cos(d), y + r1 * sin(d));
		glVertexAttrib2f(snake_coord, outer_a, b);
		glVertex2f(x + r2 * cos(d), y + r2 * sin(d));
		b += b_step_size;
	}

	glVertexAttrib3f(across, direction*cos(end), direction*sin(end), 0);
	glVertexAttrib3f(along, -direction*-sin(end), -direction*cos(end), 0);

	glVertexAttrib2f(snake_coord, inner_a, b_end);
	glVertex2f(x + r1 * cos(end), y + r1 * sin(end));
	glVertexAttrib2f(snake_coord, outer_a, b_end);
	glVertex2f(x + r2 * cos(end), y + r2 * sin(end));

	glEnd();

	glUseProgram(0);
}

void textured_skin::fat_line(ymse::vec2f p, ymse::vec2f dir, float len, float t, float b_begin, float b_end) {
	glUseProgram(d->prog.get_id());

	d->prog.set_uniform("diffuse_map", 0);
	d->prog.set_uniform("normal_map", 1);

	float &x1(p[0]), &y1(p[1]), &dx(dir[0]), &dy(dir[1]);

	// Calculate normal * thickness:
	float nx = dy * t, ny = -dx * t;
	float x2 = x1 + dx*len, y2 = y1 + dy * len;

	glVertexAttrib3f(across, nx, ny, 0);
	glVertexAttrib3f(along, -dx, -dy, 0);

	glBegin(GL_QUADS);
	glVertexAttrib2f(snake_coord, 1, b_begin);
	glVertex2f(x1 + nx, y1 + ny);
	glVertexAttrib2f(snake_coord, 1, b_end);
	glVertex2f(x2 + nx, y2 + ny);
	glVertexAttrib2f(snake_coord, -1, b_end);
	glVertex2f(x2 - nx, y2 - ny);
	glVertexAttrib2f(snake_coord, -1, b_begin);
	glVertex2f(x1 - nx, y1 - ny);
	glEnd();

	glUseProgram(0);
}

void textured_skin::metaballs(ymse::rectf bb, const std::vector<ymse::vec3f>& p) {
	glUseProgram(d->metaballs.get_id());

	glBegin(GL_QUADS);
	glVertex2f(bb.x1, bb.y1);
	glVertex2f(bb.x1, bb.y2);
	glVertex2f(bb.x2, bb.y2);
	glVertex2f(bb.x2, bb.y1);
	glEnd();

	glUseProgram(0);
}
