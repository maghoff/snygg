#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include <ymse/vec.hpp>
#include <ymse/gl/program.hpp>
#include <ymse/gl/shader.hpp>
#include "textured_skin.hpp"

struct textured_skin::impl {
	ymse::gl::program prog;
};

textured_skin::textured_skin(const std::string& path) :
	d(new impl)
{
	ymse::gl::shader vertex(GL_VERTEX_SHADER), fragment(GL_FRAGMENT_SHADER);

	vertex.source_file(path + "/vertex.glsl");
	fragment.source_file(path + "/fragment.glsl");

	d->prog.attach(vertex);
	d->prog.attach(fragment);

	d->prog.link();

	// shaders go out of scope, but are kept alive by OpenGL because of d->prog
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

	float r1 = r-t, r2 = r+t;
	float step_size = get_step_size(r2);
	if (begin > end) std::swap(begin, end);

	float &x(p[0]), &y(p[1]);

	glBegin(GL_TRIANGLE_STRIP);

	for (float d = begin; d < end; d += step_size) {
		glVertex2f(x + r1 * cos(d), y + r1 * sin(d));
		glVertex2f(x + r2 * cos(d), y + r2 * sin(d));
	}

	glVertex2f(x + r1 * cos(end), y + r1 * sin(end));
	glVertex2f(x + r2 * cos(end), y + r2 * sin(end));

	glEnd();

	glUseProgram(0);
}

void textured_skin::fat_line(ymse::vec2f p, ymse::vec2f dir, float len, float t, float b_begin, float b_end) {
	glUseProgram(d->prog.get_id());

	float &x1(p[0]), &y1(p[1]), &dx(dir[0]), &dy(dir[1]);

	// Calculate normal * thickness:
	float nx = dy * t, ny = -dx * t;
	float x2 = x1 + dx*len, y2 = y1 + dy * len;

	glBegin(GL_QUADS);
	glVertex2f(x1 + nx, y1 + ny);
	glVertex2f(x2 + nx, y2 + ny);
	glVertex2f(x2 - nx, y2 - ny);
	glVertex2f(x1 - nx, y1 - ny);
	glEnd();

	glUseProgram(0);
}
