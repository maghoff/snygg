#ifndef OPENGL_RESOURCE_HPP
#define OPENGL_RESOURCE_HPP

class opengl_resource {
public:
	opengl_resource();
	virtual ~opengl_resource();

	virtual void recreate_opengl_resources() = 0;
};

#endif // OPENGL_RESOURCE_HPP
