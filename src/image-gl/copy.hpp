#ifndef IMAGE_GL_COPY_HPP
#define IMAGE_GL_COPY_HPP

namespace image {

class surface;

namespace gl {

void copy_surface_to_texture(const surface&, unsigned int gl_texture);

}}

#endif // IMAGE_GL_COPY_HPP
