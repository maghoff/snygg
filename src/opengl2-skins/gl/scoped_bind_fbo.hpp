#ifndef SCOPED_BIND_FBO_HPP
#define SCOPED_BIND_FBO_HPP

class gl_fbo;

struct scoped_bind_fbo {
	bool is_bound;
	int prev_read_buffer, prev_draw_buffer;

	void bind(int);

public:
	explicit scoped_bind_fbo(const gl_fbo&);
	explicit scoped_bind_fbo(int);
	~scoped_bind_fbo();

	scoped_bind_fbo(const scoped_bind_fbo&) = delete;
	scoped_bind_fbo& operator = (const scoped_bind_fbo&) = delete;

	void unbind();
};

#endif // SCOPED_BIND_FBO_HPP
