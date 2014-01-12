#include <ymse/gl.h>
#include "gl_fbo.hpp"
#include "scoped_bind_fbo.hpp"

scoped_bind_fbo::scoped_bind_fbo(const gl_fbo& fbo) {
	bind(fbo.get_id());
}

scoped_bind_fbo::scoped_bind_fbo(int id) {
	bind(id);
}

scoped_bind_fbo::~scoped_bind_fbo() {
	unbind();
}

void scoped_bind_fbo::bind(int id) {
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &prev_draw_buffer);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &prev_read_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, id);
	is_bound = true;
}

void scoped_bind_fbo::unbind() {
	if (is_bound) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, prev_read_buffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, prev_draw_buffer);
		is_bound = false;
	}
}
