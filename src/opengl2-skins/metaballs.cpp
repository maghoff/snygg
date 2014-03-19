#include "metaballs.hpp"

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
#include "metaballs_accumulator.hpp"

const auto vertex = 5u;

struct metaballs::impl {
	scalable_skin* target;

	la::matrix33f transform;

	std::unique_ptr<shader_program> metaballs;
	std::unique_ptr<shader_program> mapping;

	gl_fbo fbo;

	metaballs_accumulator accumulator;

	class {
		gl::texture stored_value[2];
		int next_index = 0;

	public:
		gl::texture& prev() { return stored_value[next_index ^ 1]; }
		gl::texture& next() { return stored_value[next_index]; }
		void step_tex() { next_index ^= 1; }
	} tex;
};

metaballs::metaballs(scalable_skin* s, const std::string& path) :
	d(new impl)
{
	d->target = s;

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
	for (auto tex : { &d->tex.prev(), &d->tex.next() }) {
		glBindTexture(GL_TEXTURE_2D, tex->get_id());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, buf.data());
		assert(glGetError() == GL_NONE);
	}

	d->fbo.set_size(width, height);

	//d->target->load_opengl_resources(width, height);

	d->accumulator.clear();
}

void metaballs::set_transformation(const la::matrix33f& transform_) {
	d->target->set_transformation(transform_);
	d->transform = transform_.transposed();
}

void metaballs::blood(la::vec2f p, float r) {
	d->accumulator.add_to_generation(p, r);
}

void metaballs::update_four_metaballs(const complex_polygon& floor_poly, const la::vec4f* data) {
	d->fbo.render_to(d->tex.next().get_id());

	scoped_bind_fbo binder(d->fbo);

	glUniform4fv(glGetUniformLocation(d->metaballs->get_program_id(), "balls"), 4, reinterpret_cast<const GLfloat*>(data));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d->tex.prev().get_id());
	d->metaballs->set_uniform("storedValue", 0);

	glClear(GL_COLOR_BUFFER_BIT);
	draw(vertex, floor_poly);

	binder.unbind();

	d->tex.step_tex();
}

void metaballs::update_metaballs(const complex_polygon& floor_poly, std::vector<la::vec4f> p) {
	p.resize((p.size() + 3) / 4 * 4); // ceil(p.size() / 4) * 4

	glUseProgram(d->metaballs->get_program_id());
	glDisable(GL_BLEND);
	glUniformMatrix3fv(glGetUniformLocation(d->metaballs->get_program_id(), "transform"), 1, GL_FALSE, d->transform.v);

	for (auto i = 0u; i < p.size()/4; ++i) {
		update_four_metaballs(floor_poly, &p[i*4]);
	}

	glEnable(GL_BLEND);
	glUseProgram(0);
}

void metaballs::draw_metaballs(const complex_polygon& floor_poly) {
	glUseProgram(d->mapping->get_program_id());

	glUniformMatrix3fv(glGetUniformLocation(d->mapping->get_program_id(), "transform"), 1, GL_FALSE, d->transform.v);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d->tex.prev().get_id());

	d->mapping->set_uniform("ambient", 0.4f, 0.4f, 0.4f, 1.0f);

	draw(vertex, floor_poly);
	glUseProgram(0);
}

void metaballs::floor(const complex_polygon& floor_poly) {
	auto balls = d->accumulator.calculate_updates();
	if (!balls.empty()) update_metaballs(floor_poly, std::move(balls));
	draw_metaballs(floor_poly);

	d->accumulator.step_generation();

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
