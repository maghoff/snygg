#ifndef BUFFERING_SKIN_HPP
#define BUFFERING_SKIN_HPP

#include <memory>
#include <string>
#include <vector>
#include <scalable_skin.hpp>

struct vertexSpec;

class buffering_skin : public scalable_skin {
	unsigned buffer;

	void drawVertexSpec(const std::vector<vertexSpec>&, unsigned);

public:
	buffering_skin();
	~buffering_skin() override;

	void load_opengl_resources(int width, int height) override;

	void set_transformation(const la::matrix33f&) override;

	void circle(la::vec2f p, float r) override;
	void blood(la::vec2f p, float r) override;

	void fat_arc(la::vec2f p, float r, float t, float begin, float end, float b_begin, float b_end) override;
	void fat_line(la::vec2f p, la::vec2f d, float len, float t, float b_begin, float b_end) override;

	void cap(la::vec2f p, float snake_direction, float cap_direction, float b_coord) override;

	void floor(const complex_polygon&) override;

	void enter_state(state_t) override;
};

#endif // BUFFERING_SKIN_HPP
