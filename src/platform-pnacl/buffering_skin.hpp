#ifndef BUFFERING_SKIN_HPP
#define BUFFERING_SKIN_HPP

#include <map>
#include <string>
#include <vector>
#include <scalable_skin.hpp>
#include <matrix.hpp>

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

	void draw_geometry_spec(const geometry_spec&);

	unsigned floorProgram, colorProgram, textureProgram, fallbackProgram = 0;
	unsigned textures[2];
	void to_floor_shader();
	void to_wall_shader();
	void to_texture_shader();

	int acquire_fallback_shader(const std::map<std::string, std::vector<char>>& resources);

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
