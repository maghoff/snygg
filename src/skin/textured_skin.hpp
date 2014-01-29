#ifndef TEXTURED_SKIN_HPP
#define TEXTURED_SKIN_HPP

#include <memory>
#include <string>
#include "scalable_skin.hpp"

class shader_configuration;

class textured_skin : public scalable_skin {
	struct impl;
	std::unique_ptr<impl> d;

	void to_shader(const shader_configuration*);

	void to_no_shader();
	void to_texture_shader();
	void to_snakeskin_shader();
	void to_wall_shader();
	void to_food_shader();
	void to_floor_shader();

	void circle_core(ymse::vec2f p, float r);

public:
	textured_skin(const std::string& path);
	~textured_skin();

	void load_opengl_resources(int width, int height);

	void circle(ymse::vec2f p, float r);
	void blood(ymse::vec2f p, float r);

	void fat_arc(ymse::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end);
	void fat_line(ymse::vec2f p, ymse::vec2f d, float len, float t, float b_begin, float b_end);

	void cap(ymse::vec2f p, float snake_direction, float cap_direction, float b_coord);

	void floor(const complex_polygon&);

	void enter_state(state_t);
};

#endif // TEXTURED_SKIN_HPP
