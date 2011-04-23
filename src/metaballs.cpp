#include <ymse/gl.h>
#include <set>
#include <ymse/gl/program.hpp>
#include <ymse/gl/shader.hpp>
#include <ymse/gl/texture.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "complex_polygon.hpp"
#include "metaballs.hpp"

struct metaballs::impl {
	scalable_skin* target;

	std::string path; //< This is sooo the wrong thing to store;

	boost::scoped_ptr<ymse::gl::program> metaballs;
	ymse::gl::texture metaballs_coordinates;

	struct {
		std::set<ymse::vec3f> gen[2];
		int next_gen_index;

		std::set<ymse::vec3f>& prev_gen() { return gen[next_gen_index ^ 1]; }
		std::set<ymse::vec3f>& next_gen() { return gen[next_gen_index]; }

		void step_generation() { next_gen_index ^= 1; }
	} balls;
};

void metaballs::load_opengl_resources() {
	glGetError();
	d->metaballs.reset();
	glGetError();
	d->metaballs.reset(new ymse::gl::program);

	ymse::gl::shader mb_vertex(GL_VERTEX_SHADER), mb_fragment(GL_FRAGMENT_SHADER);

	mb_vertex.source_file(d->path + "/mb_vertex.glsl");
	mb_fragment.source_file(d->path + "/mb_fragment.glsl");

	d->metaballs->attach(mb_vertex);
	d->metaballs->attach(mb_fragment);

	d->metaballs->link();

	d->balls.next_gen_index = 0;

	//d->target->load_opengl_resources();
}

metaballs::metaballs(scalable_skin* s, const std::string& path) :
	d(new impl)
{
	d->target = s;
	d->path = path;
}

metaballs::~metaballs() {
}

void metaballs::blood(ymse::vec2f p, float r) {
	d->balls.next_gen().insert(ymse::vec3f(p[0], p[1], r));
}

void metaballs::render_metaballs(const complex_polygon& floor_poly, const std::vector<ymse::vec4f>& p) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, d->metaballs_coordinates.get_id());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16F_ARB, p.size(), 0, GL_RGBA, GL_FLOAT, &p[0]);

	glUseProgram(d->metaballs->get_id());

	d->metaballs->set_uniform("number_of_balls", (int)p.size());
	d->metaballs->set_uniform("balls", 0);

	floor_poly.draw();

	glUseProgram(0);
}

void metaballs::floor(const complex_polygon& floor_poly) {
	std::vector<ymse::vec3f> remove, add;
	std::set_difference(
		d->balls.prev_gen().begin(), d->balls.prev_gen().end(),
		d->balls.next_gen().begin(), d->balls.next_gen().end(),
		std::back_inserter(remove)
	);
	std::set_difference(
		d->balls.next_gen().begin(), d->balls.next_gen().end(),
		d->balls.prev_gen().begin(), d->balls.prev_gen().end(),
		std::back_inserter(add)
	);

	std::vector<ymse::vec4f> balls;
	for (std::vector<ymse::vec3f>::const_iterator it = remove.begin(); it != remove.end(); ++it) {
		balls.push_back(ymse::vec4f(it->v[0], it->v[1], it->v[2], -1));
	}
	for (std::vector<ymse::vec3f>::const_iterator it = add.begin(); it != add.end(); ++it) {
		balls.push_back(ymse::vec4f(it->v[0], it->v[1], it->v[2], 1));
	}
	render_metaballs(floor_poly, balls);

	d->balls.step_generation();
	d->balls.next_gen().clear();

	d->target->floor(floor_poly);
}

void metaballs::circle(ymse::vec2f p, float r) {
	d->target->circle(p, r);
}

void metaballs::fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
	d->target->fat_arc(p, r, t, begin, end, b_begin, b_end);
}

void metaballs::fat_line(ymse::vec2f p, ymse::vec2f dir, float len, float t, float b_begin, float b_end) {
	d->target->fat_line(p, dir, len, t, b_begin, b_end);
}

void metaballs::cap(ymse::vec2f p, float snake_direction, float cap_direction, float b_coord) {
	d->target->cap(p, snake_direction, cap_direction, b_coord);
}

void metaballs::set_pixels_per_unit(float ppu) {
	scalable_skin::set_pixels_per_unit(ppu);
	d->target->set_pixels_per_unit(ppu);
}
