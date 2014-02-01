#ifndef IMAGE_IMG_LOAD_HPP
#define IMAGE_IMG_LOAD_HPP

#include <string>

namespace image {

class surface;

surface load_jpeg(const std::string& filename);

}

#endif // IMAGE_IMG_LOAD_HPP
