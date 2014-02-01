#ifndef IMAGE_SURFACE_HPP
#define IMAGE_SURFACE_HPP

#include <vector>

namespace image {

class surface {
	std::vector<unsigned char> mdata;
	unsigned int mwidth, mheight, mcomponents;

public:
	surface(const std::vector<unsigned char>& data, unsigned int width, unsigned int height, unsigned int components);
	surface(std::vector<unsigned char>&& data, unsigned int width, unsigned int height, unsigned int components);

	surface(const surface&) = delete;
	surface& operator = (const surface&) = delete;

	surface(surface&&) = default;
	surface& operator = (surface&&) = default;

	unsigned int width() const { return mwidth; }
	unsigned int height() const { return mheight; }
	unsigned int components() const { return mcomponents; }

	const std::vector<unsigned char>& data() const { return mdata; }
};

}

#endif // IMAGE_SURFACE_HPP
