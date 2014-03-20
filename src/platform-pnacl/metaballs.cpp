#include "metaballs.hpp"
#include <cassert>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "renderable_complex_polygon.hpp"


#include <vector>
#include <iosfwd>
GLuint buildShaderProgram(
	const std::vector<std::pair<GLenum, std::vector<std::vector<char>>>>& shaders,
	const std::vector<std::pair<std::string, GLuint>>& attribs,
	std::ostream& out
);


enum attrib {
	vertex = 0
};


metaballs::tex_generation_manager::tex_generation_manager() {
	glGenTextures(2, stored_value);
}

metaballs::tex_generation_manager::~tex_generation_manager() {
	glDeleteTextures(2, stored_value);
}

template <typename T>
static const T& get(const std::map<std::string, T>& resources, const std::string& key) {
	auto i = resources.find(key);
	if (i == resources.end()) throw std::runtime_error("Requested missing resource");
	return i->second;
}

metaballs::metaballs(const std::map< std::string, std::vector< char > >& resources, std::ostream& out) {
	metaballsProgram = buildShaderProgram(
		{
			{ GL_VERTEX_SHADER, { get(resources, "mb_vertex.glsl") } },
			{ GL_FRAGMENT_SHADER, { get(resources, "mb_function.glsl") } }
		}, {
			{ "vertex", attrib::vertex }
		},
		out
	);
	if (metaballsProgram == 0) throw std::runtime_error("Shader linking failed");
	glGenFramebuffers(1, &metaballsFBO);


	metaballsMappingProgram = buildShaderProgram(
		{
			{ GL_VERTEX_SHADER, { get(resources, "mb_vertex.glsl") } },
			{ GL_FRAGMENT_SHADER, {
				get(resources, "light.glsl"),
				get(resources, "mb_mapping.glsl"),
			} }
		}, {
			{ "vertex", attrib::vertex }
		},
		out
	);
	if (metaballsMappingProgram == 0) throw std::runtime_error("Shader linking failed");


}

metaballs::~metaballs() {
	glDeleteFramebuffers(1, &metaballsFBO);
}


void metaballs::circle(la::vec2f p, float r) {}
void metaballs::fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) {}
void metaballs::fat_line(la::vec2f p, la::vec2f d, float len, float t, float b_begin, float b_end) {}
void metaballs::cap(la::vec2f p, float snake_direction, float cap_direction, float b_coord) {}
void metaballs::enter_state(state_t) {}

void metaballs::load_opengl_resources(int width_, int height_) {
	width = width_;
	height = height_;

	for (auto id : { tex.prev(), tex.next() }) {
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (width + 3)/4, height, 0, GL_RGBA, GL_FLOAT, 0);
		assert(glGetError() == GL_NONE);
	}
	acc.clear();
}

void metaballs::set_transformation(const la::matrix33f& transform_) {
	transform = transform_;
}

void metaballs::blood(la::vec2f p, float r) {
	acc.add_to_generation(p, r);
}

void metaballs::update_four_metaballs(const renderable_complex_polygon& floor, const la::vec4f* data) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.next(), 0);

	glUniform4fv(glGetUniformLocation(metaballsProgram, "balls"), 4, reinterpret_cast<const GLfloat*>(data));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex.prev());
	glUniform1i(glGetUniformLocation(metaballsProgram, "storedValue"), 0);

	floor.render(attrib::vertex);

	tex.step();
}

void metaballs::update_metaballs(const renderable_complex_polygon& floor, std::vector<la::vec4f> p) {
	if (p.empty()) return;
	p.resize((p.size() + 3) / 4 * 4); // ceil(p.size() / 4) * 4

	glBindFramebuffer(GL_FRAMEBUFFER, metaballsFBO);
	glViewport(0, 0, (width+3)/4, height);

	glUseProgram(metaballsProgram);
	glUniformMatrix3fv(glGetUniformLocation(metaballsProgram, "transform"), 1, GL_FALSE, transform.v);

	auto one_px = 2. / width;
	auto delta_x = one_px / transform[0][0];
	glUniform1f(glGetUniformLocation(metaballsProgram, "delta_x"), delta_x);

	for (auto i = 0u; i < p.size()/4; ++i) {
		update_four_metaballs(floor, &p[i*4]);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}

void metaballs::draw_metaballs(const renderable_complex_polygon& floor) {
	glUseProgram(metaballsMappingProgram);
	glUniformMatrix3fv(glGetUniformLocation(metaballsMappingProgram, "transform"), 1, GL_FALSE, transform.v);
	glUniform1f(glGetUniformLocation(metaballsMappingProgram, "screen_width"), (width + 3) / 4 * 4);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex.prev());
	glUniform1i(glGetUniformLocation(metaballsMappingProgram, "storedValue"), 0);

	glUniform4f(glGetUniformLocation(metaballsMappingProgram, "ambient"), 0.4f, 0.4f, 0.4f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFuncSeparate(
		GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
		GL_ONE, GL_ONE
	);
	floor.render(attrib::vertex);
	glDisable(GL_BLEND);
}

void metaballs::floor(const complex_polygon& floor_poly) {
	throw std::logic_error("Wrong metaballs::floor called. Use renderable_complex_polygon overload!");
}

void metaballs::floor(const renderable_complex_polygon& floor) {
	auto balls = acc.calculate_updates();
	if (!balls.empty()) update_metaballs(floor, std::move(balls));
	draw_metaballs(floor);

	acc.step_generation();
}

