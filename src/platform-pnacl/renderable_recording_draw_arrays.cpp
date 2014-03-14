#include "renderable_recording_draw_arrays.hpp"
#include <GLES2/gl2.h>
#include "geometry_spec.hpp"
#include "buffering_skin.hpp"

void renderable_recording_draw_arrays::update(const std::vector<geometry_spec>& spec) {
	if (spec.size() > buffers.size()) {
		auto old_size = buffers.size();
		buffers.resize(spec.size());
		glGenBuffers(spec.size() - old_size, buffers.data() + old_size);
	} else if (spec.size() < buffers.size()) {
		glDeleteBuffers(buffers.size() - spec.size(), buffers.data() + spec.size());
	}
	details.resize(spec.size());

	for (decltype(spec.size()) i = 0; i < spec.size(); ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferData(GL_ARRAY_BUFFER, spec[i].data.size() * sizeof(spec[i].data[0]), spec[i].data.data(), GL_STATIC_DRAW);
		details[i].first = spec[i].data.size();
		details[i].second = spec[i].mode;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void renderable_recording_draw_arrays::render(buffering_skin& skin) {
	for (decltype(buffers.size()) i = 0; i < buffers.size(); ++i) {
		skin.draw_arrays(buffers[i], details[i].first, details[i].second);
	}
}
