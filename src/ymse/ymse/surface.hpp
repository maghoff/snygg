#ifndef YMSE_SURFACE_HPP
#define YMSE_SURFACE_HPP

#include <vector>

namespace ymse {

namespace gl {
	class texture;
}

class surface {
	std::vector<unsigned char> data;
	unsigned int width, height, components;

public:
	surface(const std::vector<unsigned char>& data, unsigned int width, unsigned int height, unsigned int components);
	surface(std::vector<unsigned char>&& data, unsigned int width, unsigned int height, unsigned int components);

	surface(const surface&) = delete;
	surface& operator = (const surface&) = delete;

	surface(surface&&) = default;
	surface& operator = (surface&&) = default;

	void copy_to(gl::texture&) const;
};

}

#endif // YMSE_SURFACE_HPP
