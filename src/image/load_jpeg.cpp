#include <vector>
#include <stdexcept>
#include <string>
#include "surface.hpp"
//#include "load_jpeg.hpp"

#include <stdio.h>
extern "C" {
#include "jpeglib.h"
}

namespace image {

surface load_jpeg(const std::string& filename) {
	jpeg_error_mgr jerr;
	jpeg_decompress_struct cinfo;
	FILE * infile;

	if ((infile = fopen(filename.c_str(), "rb")) == NULL) {
		throw std::runtime_error("can't open " + filename);
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);

	(void) jpeg_read_header(&cinfo, TRUE);
	/* We can ignore the return value from jpeg_read_header since
		(a) suspension is not possible with the stdio data source, and
		(b) we passed TRUE to reject a tables-only JPEG file as an error.
	See libjpeg.doc for more info. */

	// Step 4: set parameters for decompression
	// In this example, we don't need to change any of the defaults set by jpeg_read_header(), so we do nothing here.

	(void) jpeg_start_decompress(&cinfo);
	// We can ignore the return value since suspension is not possible with the stdio data source.

	unsigned int row_stride = cinfo.output_width * cinfo.output_components;

	std::vector<unsigned char> raw_buffer(row_stride * cinfo.output_height);
	unsigned char* buffer[cinfo.output_height];
	for (unsigned int i=0; i < cinfo.output_height; ++i) buffer[i] = raw_buffer.data() + row_stride * i;

	while (cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, buffer + cinfo.output_scanline, cinfo.output_height - cinfo.output_scanline);
	}

	(void) jpeg_finish_decompress(&cinfo);
	// We can ignore the return value since suspension is not possible with the stdio data source.

	jpeg_destroy_decompress(&cinfo);


	return surface(std::move(raw_buffer), cinfo.output_width, cinfo.output_height, cinfo.output_components);
}

}
