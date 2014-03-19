#ifndef BUFFERING_SKIN_HPP
#define BUFFERING_SKIN_HPP

#include <map>
#include <string>
#include <vector>
#include <scalable_skin.hpp>
#include <matrix.hpp>
#include "metaballs_accumulator.hpp"

enum class geometry_mode;
struct geometry_spec;
class renderable_complex_polygon;

namespace image {
	class surface;
}

class buffering_skin : public scalable_skin {
	std::ostream& out;

	unsigned buffer;
	la::matrix33f transform;

	metaballs_accumulator metaballs;

	void draw_geometry_spec(const geometry_spec&);

	unsigned floorProgram, colorProgram, textureProgram, metaballsProgram, metaballsMappingProgram;
	unsigned textures[2];
	void to_floor_shader();
	void to_wall_shader();
	void to_texture_shader();

	unsigned metaballsFBO;
	class tex_generation_manager {
		unsigned stored_value[2];
		int next_index = 0;

	public:
		tex_generation_manager();
		~tex_generation_manager();

		unsigned prev() { return stored_value[next_index ^ 1]; }
		unsigned next() { return stored_value[next_index]; }
		void step() { next_index ^= 1; }
	} tex;

	void update_four_metaballs(const renderable_complex_polygon&, const la::vec4f*);
	void update_metaballs(const renderable_complex_polygon&, std::vector<la::vec4f>);
	void draw_metaballs(const renderable_complex_polygon&);

public:
	buffering_skin(
		const std::map<std::string, std::vector<char>>& resources,
		const std::map<std::string, image::surface>& images,
		std::ostream& debug_output
	);
	~buffering_skin() override;

	void draw_arrays(unsigned buffer_object, unsigned n, geometry_mode mode);
	void draw_elements(unsigned vertex_buffer_object, unsigned index_buffer_object, unsigned elements);

	void load_opengl_resources(int width, int height) override;

	void set_transformation(const la::matrix33f&) override;

	void circle(la::vec2f p, float r) override;
	void blood(la::vec2f p, float r) override;

	void fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) override;
	void fat_line(la::vec2f p, la::vec2f d, float len, float t, float b_begin, float b_end) override;

	void cap(la::vec2f p, float snake_direction, float cap_direction, float b_coord) override;

	void floor(const complex_polygon&) override;
	void floor(const renderable_complex_polygon&);

	void enter_state(state_t) override;
};

#endif // BUFFERING_SKIN_HPP
