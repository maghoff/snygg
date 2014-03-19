#ifndef RENDERABLE_COMPLEX_POLYGON_HPP
#define RENDERABLE_COMPLEX_POLYGON_HPP

#include <GLES2/gl2.h>
#include <array>

struct complex_polygon;

class renderable_complex_polygon {
	std::array<GLuint, 2> buffers;
	GLsizei points;

public:
	renderable_complex_polygon();
	explicit renderable_complex_polygon(const complex_polygon&);

	renderable_complex_polygon(const renderable_complex_polygon&) = delete;
	renderable_complex_polygon& operator = (const renderable_complex_polygon&) = delete;

	renderable_complex_polygon(renderable_complex_polygon&&);
	renderable_complex_polygon& operator = (renderable_complex_polygon&&);

	~renderable_complex_polygon();

	void render(GLint vertexLocation) const;
};

#endif
