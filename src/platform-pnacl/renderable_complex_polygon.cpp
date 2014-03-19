#include "renderable_complex_polygon.hpp"
#include <complex_polygon.hpp>
#include <vec.hpp>

renderable_complex_polygon::renderable_complex_polygon() :
	buffers{{0, 0}},
	points{0}
{
}

renderable_complex_polygon::renderable_complex_polygon(const complex_polygon& polygon) {
	glGenBuffers(2, buffers.data());

	const auto& vertexBuffer = buffers[0];
	std::vector<GLfloat> coordinates;
	for (const auto& p : polygon.points) {
		coordinates.push_back(p.x());
		coordinates.push_back(p.y());
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(coordinates[0]), coordinates.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	const auto& elementBuffer = buffers[1];
	std::vector<GLshort> indices(polygon.triangles.begin(), polygon.triangles.end());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	points = indices.size();
}

renderable_complex_polygon& renderable_complex_polygon::operator=(renderable_complex_polygon&& rhs) {
	std::swap(buffers, rhs.buffers);
	points = rhs.points;
	return *this;
}

renderable_complex_polygon::renderable_complex_polygon(renderable_complex_polygon&& rhs) :
	buffers(rhs.buffers),
	points(rhs.points)
{
	rhs.buffers = {{0, 0}};
}

renderable_complex_polygon::~renderable_complex_polygon() {
	glDeleteBuffers(2, buffers.data());
}

void renderable_complex_polygon::render(GLint vertexLocation) const {
	const auto& vertexBuffer = buffers[0];
	const auto& elementBuffer = buffers[1];

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glDrawElements(GL_TRIANGLES, points, GL_UNSIGNED_SHORT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
