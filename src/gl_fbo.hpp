#ifndef SIMPLEMODEL_GL_FBO_HPP
#define SIMPLEMODEL_GL_FBO_HPP

#include <boost/noncopyable.hpp>

class gl_fbo : public boost::noncopyable {
	unsigned id;
	unsigned depthbuffer;

	void init();

public:
	gl_fbo();
	gl_fbo(int width, int height);
	~gl_fbo();

	void set_size(int width, int height);

	unsigned get_id() const;
	void render_to(unsigned);
};

#endif
