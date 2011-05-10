#ifndef SCOPED_BIND_FBO_HPP
#define SCOPED_BIND_FBO_HPP

#include <boost/noncopyable.hpp>

class gl_fbo;

struct scoped_bind_fbo : boost::noncopyable {
	bool is_bound;
	int prev_read_buffer, prev_draw_buffer;

	void bind(int);

public:
	explicit scoped_bind_fbo(const gl_fbo&);
	explicit scoped_bind_fbo(int);
	~scoped_bind_fbo();

	void unbind();
};

#endif // SCOPED_BIND_FBO_HPP
