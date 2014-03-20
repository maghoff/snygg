#include <cassert>
#include <GL/glew.h>

#include "scoped_bind_fbo.hpp"
#include "gl_fbo.hpp"

gl_fbo::gl_fbo() {
	glGenFramebuffersEXT(1, &id);
}

gl_fbo::~gl_fbo() {
	glDeleteFramebuffersEXT(1, &id);
}

GLuint gl_fbo::get_id() const {
	return id;
}

void gl_fbo::render_to(unsigned tex_id) {
	scoped_bind_fbo binder(id);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex_id, 0);
	glBindTexture(GL_TEXTURE_2D, tex_id);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	(void)status;
	assert(status == GL_FRAMEBUFFER_COMPLETE_EXT);
}


void gl_fbo_multisample::init() {
	glGenRenderbuffers(1, &colorbuffer);
	glGenFramebuffers(1, &id);
}

gl_fbo_multisample::gl_fbo_multisample() {
	init();
}

gl_fbo_multisample::gl_fbo_multisample(int width, int height) {
	init();
	set_size(width, height);
}

void gl_fbo_multisample::set_size(int width_, int height_) {
	GLint samples;
	glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &samples);

	width = width_;
	height = height_;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_SRGB8_ALPHA8, width, height);

	assert(id != 0);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorbuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	(void)status;
	assert(status == GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glPopAttrib();

	assert(glGetError() == GL_NONE);
}

gl_fbo_multisample::~gl_fbo_multisample() {
	glDeleteFramebuffers(1, &id);
}

GLuint gl_fbo_multisample::get_id() const {
	return id;
}

void gl_fbo_multisample::blit_to(gl_fbo& target) {
	scoped_bind_fbo binder(0); //< Uses just to store
	glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target.get_id());

	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
