#include <cassert>
#include <ymse/gl.h>
#include "scoped_bind_fbo.hpp"
#include "gl_fbo.hpp"

void gl_fbo::init() {
	glGenRenderbuffersEXT(1, &depthbuffer);
	glGenFramebuffersEXT(1, &id);
}

gl_fbo::gl_fbo() {
	init();
}

gl_fbo::gl_fbo(int width, int height) {
	init();
}

void gl_fbo::set_size(int width, int height) {
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glBindRenderbufferEXT(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	assert(id != 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthbuffer);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glPopAttrib();
}

gl_fbo::~gl_fbo() {
	glDeleteFramebuffersEXT(1, &id);
	glDeleteRenderbuffersEXT(1, &depthbuffer);
}

GLuint gl_fbo::get_id() const {
	return id;
}

void gl_fbo::render_to(unsigned tex_id) {
	scoped_bind_fbo binder(id);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex_id, 0);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	assert(status == GL_FRAMEBUFFER_COMPLETE_EXT);
}
