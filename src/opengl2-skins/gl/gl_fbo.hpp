#ifndef SIMPLEMODEL_GL_FBO_HPP
#define SIMPLEMODEL_GL_FBO_HPP

class gl_fbo {
	unsigned id;
	unsigned depthbuffer;

	void init();

public:
	gl_fbo();
	gl_fbo(int width, int height);
	~gl_fbo();

	gl_fbo(const gl_fbo&) = delete;
	gl_fbo& operator = (const gl_fbo&) = delete;

	void set_size(int width, int height);

	unsigned get_id() const;
	void render_to(unsigned);
};

class gl_fbo_multisample {
	unsigned id;
	unsigned colorbuffer, depthbuffer;
	unsigned width, height;

	void init();

public:
	gl_fbo_multisample();
	gl_fbo_multisample(int width, int height);
	~gl_fbo_multisample();

	gl_fbo_multisample(const gl_fbo_multisample&) = delete;
	gl_fbo_multisample& operator = (const gl_fbo_multisample&) = delete;

	void set_size(int width, int height);

	unsigned get_id() const;
	void blit_to(gl_fbo&);
};

#endif
