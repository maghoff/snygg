#include <GL/gl.h>
#include <ymse/gl/program.hpp>
#include <ymse/gl/shader.hpp>
#include <ymse/gl/texture.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "metaballs.hpp"

struct metaballs::impl {
	ymse::gl::program metaballs;
	ymse::gl::texture metaballs_coordinates;

	std::vector<ymse::vec3f> balls;
};

metaballs::metaballs(const std::string& path) :
	textured_skin(path),
	d(new impl)
{
	ymse::gl::shader mb_vertex(GL_VERTEX_SHADER), mb_fragment(GL_FRAGMENT_SHADER);

	mb_vertex.source_file(path + "/mb_vertex.glsl");
	mb_fragment.source_file(path + "/mb_fragment.glsl");

	d->metaballs.attach(mb_vertex);
	d->metaballs.attach(mb_fragment);

	d->metaballs.link();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, d->metaballs_coordinates.get_id());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

metaballs::~metaballs() {
}

void metaballs::blood(ymse::vec2f p, float r) {
	d->balls.push_back(ymse::vec3f(p[0], p[1], r));
}

void metaballs::render_metaballs(ymse::rectf bb, const std::vector<ymse::vec3f>& p) {
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB16F, p.size(), 0, GL_RGB, GL_FLOAT, &p[0]);

	glUseProgram(d->metaballs.get_id());

	d->metaballs.set_uniform<int>("number_of_balls", p.size());
	d->metaballs.set_uniform<int>("balls", 0);

	glBegin(GL_QUADS);
	glVertex2f(bb.x1, bb.y1);
	glVertex2f(bb.x1, bb.y2);
	glVertex2f(bb.x2, bb.y2);
	glVertex2f(bb.x2, bb.y1);
	glEnd();

	glUseProgram(0);
}

void metaballs::finish_frame(ymse::rectf bb) {
	render_metaballs(bb, d->balls);
	d->balls.clear();
}
