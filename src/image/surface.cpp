#include "surface.hpp"

namespace image {

surface::surface(const std::vector<unsigned char>& data_, unsigned int width_, unsigned int height_, unsigned int components_) :
	mdata(data_),
	mwidth(width_),
	mheight(height_),
	mcomponents(components_)
{}

surface::surface(std::vector<unsigned char>&& data_, unsigned int width_, unsigned int height_, unsigned int components_) :
	mdata(data_),
	mwidth(width_),
	mheight(height_),
	mcomponents(components_)
{}

}
