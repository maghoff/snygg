#ifndef IMAGE_IMG_LOAD_HPP
#define IMAGE_IMG_LOAD_HPP

#include <iosfwd>

namespace image {

class surface;

surface load_jpeg(std::istream&);

}

#endif // IMAGE_IMG_LOAD_HPP
