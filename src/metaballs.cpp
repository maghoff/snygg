#include <ymse/gl.h>
#include <set>
#include <ymse/gl/program.hpp>
#include <ymse/gl/shader.hpp>
#include <ymse/gl/texture.hpp>
#include <ymse/rect.hpp>
#include <ymse/vec.hpp>
#include "complex_polygon.hpp"
#include "metaballs.hpp"
#include "plain_skin.hpp"
#include "textured_skin.hpp"

template <class BaseSkin>
struct metaballs<BaseSkin>::impl {
	std::string path; //< This is sooo the wrong thing to store;

	boost::scoped_ptr<ymse::gl::program> metaballs;
	ymse::gl::texture metaballs_coordinates;

	struct {
		std::set<ymse::vec3f> permanent;
		std::set<ymse::vec3f> gen[2];
		int next_gen_index;

		std::set<ymse::vec3f>& prev_gen() { return gen[next_gen_index ^ 1]; }
		std::set<ymse::vec3f>& next_gen() { return gen[next_gen_index]; }

		void step_generation() { next_gen_index ^= 1; }
	} balls;
};

template <class BaseSkin>
void metaballs<BaseSkin>::init(const std::string& path) {
	d->path = path;
}

template <class BaseSkin>
void metaballs<BaseSkin>::load_opengl_resources() {
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

	BaseSkin::load_opengl_resources();
}


template <>
metaballs<plain_skin>::metaballs(const std::string& path) :
	d(new impl)
{
	init(path);
}

template <>
metaballs<textured_skin>::metaballs(const std::string& path) :
	textured_skin(path),
	d(new impl)
{
	init(path);
}

template <class BaseSkin>
metaballs<BaseSkin>::~metaballs() {
}

template <class BaseSkin>
void metaballs<BaseSkin>::blood(ymse::vec2f p, float r) {
	d->balls.next_gen().insert(ymse::vec3f(p[0], p[1], r));
}

template <class BaseSkin>
void metaballs<BaseSkin>::render_metaballs(const complex_polygon& floor_poly, const std::vector<ymse::vec3f>& p) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, d->metaballs_coordinates.get_id());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB16F_ARB, p.size(), 0, GL_RGB, GL_FLOAT, &p[0]);

	glUseProgram(d->metaballs->get_id());

	d->metaballs->set_uniform("number_of_balls", (int)p.size());
	d->metaballs->set_uniform("balls", 0);

	floor_poly.draw();

	glUseProgram(0);
}

template <class BaseSkin>
void metaballs<BaseSkin>::floor(const complex_polygon& floor_poly) {
	std::vector<ymse::vec3f> balls;
	std::copy(d->balls.next_gen().begin(), d->balls.next_gen().end(), std::back_inserter(balls));
	render_metaballs(floor_poly, balls);
	d->balls.step_generation();
	d->balls.next_gen().clear();

	BaseSkin::floor(floor_poly);
}

template class metaballs<plain_skin>;
template class metaballs<textured_skin>;
