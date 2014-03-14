#include "renderable_recording_draw_elements.hpp"
#include <GLES2/gl2.h>
#include "geometry_spec.hpp"
#include "geometry_mode.hpp"
#include "buffering_skin.hpp"
#include <limits>

renderable_recording_draw_elements::renderable_recording_draw_elements() :
	buffers{{0, 0}},
	elements(0)
{
}

static void add_triangle_strip(std::vector<vertex_spec>& vertices, std::vector<GLshort>& indices, const std::vector<vertex_spec>& data) {
	auto start_index = vertices.size();
	vertices.insert(vertices.end(), data.begin(), data.end());
	if (vertices.size() > std::numeric_limits<GLshort>::max()) throw std::runtime_error("Too many vertices");
	for (GLshort i = 2; i < data.size(); ++i) {
		indices.push_back(start_index + i-2);
		indices.push_back(start_index + i-1);
		indices.push_back(start_index + i);
	}
}

static void add_triangle_fan(std::vector<vertex_spec>& vertices, std::vector<GLshort>& indices, const std::vector<vertex_spec>& data) {
	auto start_index = vertices.size();
	vertices.insert(vertices.end(), data.begin(), data.end());
	if (vertices.size() > std::numeric_limits<GLshort>::max()) throw std::runtime_error("Too many vertices");
	for (GLshort i = 2; i < data.size(); ++i) {
		indices.push_back(start_index);
		indices.push_back(start_index + i-1);
		indices.push_back(start_index + i);
	}
}

void renderable_recording_draw_elements::update(const std::vector<geometry_spec>& spec) {
	if (buffers[0] == 0) {
		glGenBuffers(2, buffers.data());
	}

	std::vector<vertex_spec> vertices;
	std::vector<GLshort> indices;

	for (const auto& s : spec) {
		switch (s.mode) {
		case geometry_mode::triangle_strip:
			add_triangle_strip(vertices, indices, s.data);
			break;
		case geometry_mode::triangle_fan:
			add_triangle_fan(vertices, indices, s.data);
			break;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	elements = indices.size();
}

void renderable_recording_draw_elements::render(buffering_skin& skin) {
	skin.draw_elements(buffers[0], buffers[1], elements);
}
