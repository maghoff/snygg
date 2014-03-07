#include <GL/glew.h>

#include <algorithm>
#include <set>
#include <cassert>
#include <program.hpp>
#include <shader.hpp>
#include <texture.hpp>
#include <rect.hpp>
#include <vec.hpp>
#include "gl/gl_fbo.hpp"
#include "gl/scoped_bind_fbo.hpp"
#include "gl/shader_program.hpp"
#include "gl/shader_builder.hpp"
#include "draw_complex_polygon.hpp"
#include "metaballs.hpp"

#include "ball_insert_iterator.ipp"

const auto vertex = 5u;

struct metaballs::impl {
	scalable_skin* target;

	la::matrix33f transform;

	std::unique_ptr<shader_program> metaballs;
	gl::texture metaballs_coordinates;

	std::unique_ptr<shader_program> mapping;

	gl_fbo fbo;

	struct {
		std::multiset<la::vec3f> gen[2];
		int next_gen_index;

		gl::texture stored_value[2];
		int next_tex_index;

		std::multiset<la::vec3f>& prev_gen() { return gen[next_gen_index ^ 1]; }
		std::multiset<la::vec3f>& next_gen() { return gen[next_gen_index]; }
		void step_generation() { next_gen_index ^= 1; }

		gl::texture& prev_tex() { return stored_value[next_tex_index ^ 1]; }
		gl::texture& next_tex() { return stored_value[next_tex_index]; }
		void step_tex() { next_tex_index ^= 1; }
	} balls;
};

metaballs::metaballs(scalable_skin* s, const std::string& path) :
	d(new impl)
{
	d->target = s;
	d->balls.next_tex_index = 0;
	d->balls.next_gen_index = 0;

	{
		shader_builder sb;
		sb.add_shader_from_file(GL_VERTEX_SHADER, path + "/mb_vertex.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/mb_function.glsl");
		sb.bind_attrib_location(vertex, "vertex");
		d->metaballs.reset(new shader_program(sb));
	}

	{
		shader_builder sb;
		sb.add_shader_from_file(GL_VERTEX_SHADER, path + "/mb_vertex.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/mb_mapping.glsl");
		sb.add_shader_from_file(GL_FRAGMENT_SHADER, path + "/light.glsl");
		sb.bind_attrib_location(vertex, "vertex");
		d->mapping.reset(new shader_program(sb));
	}
}

metaballs::~metaballs() {
}

void metaballs::load_opengl_resources(int width, int height) {
	d->metaballs->recreate_opengl_resources();
	d->mapping->recreate_opengl_resources();

	const size_t sz = width*height*4; // 4 == sizeof(GL_R32F);
	std::vector<char> buf(sz, 0);
	for (int i=0; i<2; ++i) {
		glBindTexture(GL_TEXTURE_2D, d->balls.stored_value[i].get_id());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, buf.data());
		assert(glGetError() == GL_NONE);

		d->balls.gen[i].clear();
	}

	d->fbo.set_size(width, height);

	//d->target->load_opengl_resources(width, height);
}

void metaballs::set_transformation(const la::matrix33f& transform_) {
	d->target->set_transformation(transform_);
	d->transform = transform_.transposed();
}

void metaballs::blood(la::vec2f p, float r) {
	d->balls.next_gen().insert(la::vec3f(p[0], p[1], r));
}

void metaballs::update_metaballs(const complex_polygon& floor_poly, const std::vector<la::vec4f>& p) {
	d->fbo.render_to(d->balls.next_tex().get_id());

	glDisable(GL_BLEND);

	scoped_bind_fbo binder(d->fbo);

	glUseProgram(d->metaballs->get_program_id());

	glUniformMatrix3fv(glGetUniformLocation(d->metaballs->get_program_id(), "transform"), 1, GL_FALSE, d->transform.v);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_1D, d->metaballs_coordinates.get_id());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA16F_ARB, p.size(), 0, GL_RGBA, GL_FLOAT, &p[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, d->balls.prev_tex().get_id());

	d->metaballs->set_uniform("number_of_balls", (int)p.size());
	d->metaballs->set_uniform("balls", 0);
	d->metaballs->set_uniform("storedValue", 1);

	glClear(GL_COLOR_BUFFER_BIT);
	draw(vertex, floor_poly);

	glUseProgram(0);
	binder.unbind();
	glEnable(GL_BLEND);

	d->balls.step_tex();
}

void metaballs::draw_metaballs(const complex_polygon& floor_poly) {
	glUseProgram(d->mapping->get_program_id());

	glUniformMatrix3fv(glGetUniformLocation(d->mapping->get_program_id(), "transform"), 1, GL_FALSE, d->transform.v);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d->balls.prev_tex().get_id());

	d->mapping->set_uniform("ambient", 0.4f, 0.4f, 0.4f, 1.0f);

	draw(vertex, floor_poly);
	glUseProgram(0);
}

void metaballs::floor(const complex_polygon& floor_poly) {
	std::vector<la::vec4f> balls;

	// Removed balls:
	std::set_difference(
		d->balls.prev_gen().begin(), d->balls.prev_gen().end(),
		d->balls.next_gen().begin(), d->balls.next_gen().end(),
		ball_insert_iterator(balls, -1)
	);

	// Added balls:
	std::set_difference(
		d->balls.next_gen().begin(), d->balls.next_gen().end(),
		d->balls.prev_gen().begin(), d->balls.prev_gen().end(),
		ball_insert_iterator(balls, 1)
	);

	if (!balls.empty()) update_metaballs(floor_poly, balls);
	draw_metaballs(floor_poly);

	d->balls.step_generation();
	d->balls.next_gen().clear();

	d->target->floor(floor_poly);
}

void metaballs::circle(la::vec2f p, float r) {
	d->target->circle(p, r);
}

void metaballs::fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {
	d->target->fat_arc(p, r, t, begin, end, b_begin, b_end);
}

void metaballs::fat_line(la::vec2f p, la::vec2f dir, float len, float t, float b_begin, float b_end) {
	d->target->fat_line(p, dir, len, t, b_begin, b_end);
}

void metaballs::cap(la::vec2f p, float snake_direction, float cap_direction, float b_coord) {
	d->target->cap(p, snake_direction, cap_direction, b_coord);
}

void metaballs::set_pixels_per_unit(float ppu) {
	scalable_skin::set_pixels_per_unit(ppu);
	d->target->set_pixels_per_unit(ppu);
}

void metaballs::enter_state(state_t st) {
	d->target->enter_state(st);
}
