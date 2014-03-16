#include "load_jpeg.hpp"
#include <vector>
#include <stdexcept>
#include <string>
#include <istream>
#include "surface.hpp"

#include <stdio.h>
extern "C" {
#include "jpeglib.h"
}

namespace image {

namespace istream_source {

struct decompress_struct : jpeg_decompress_struct {
	std::istream& in;
	std::vector<char> buf;

	decompress_struct(std::istream& in_) : in(in_), buf(4096) {}
};

static void init_source(j_decompress_ptr cinfo) {
	decompress_struct* d = static_cast<decompress_struct*>(cinfo);
	d->src->bytes_in_buffer = 0;
}

static boolean fill_input_buffer(j_decompress_ptr cinfo) {
	decompress_struct* d = static_cast<decompress_struct*>(cinfo);

	if (d->in.eof()) throw std::runtime_error("jpeg not finished within file");

	d->in.read(d->buf.data(), d->buf.size());
	d->src->next_input_byte = reinterpret_cast<JOCTET*>(d->buf.data());
	d->src->bytes_in_buffer = d->in.gcount();

	return TRUE;
}

static void skip_input_data(j_decompress_ptr cinfo, long num_bytes) {
	if (num_bytes <= 0) return;

	decompress_struct* d = static_cast<decompress_struct*>(cinfo);

	auto skipped = std::min(static_cast<size_t>(num_bytes), d->src->bytes_in_buffer);
	d->src->next_input_byte += skipped;
	d->src->bytes_in_buffer -= skipped;
	num_bytes -= skipped;

	d->in.seekg(num_bytes, std::ios::cur);
}

static void term_source(j_decompress_ptr cinfo) {
}

}

surface load_jpeg(std::istream& in) {
	istream_source::decompress_struct cinfo(in);
	jpeg_error_mgr jerr;

	jpeg_source_mgr istream_source_mgr;
	istream_source_mgr.init_source = &istream_source::init_source;
	istream_source_mgr.fill_input_buffer = &istream_source::fill_input_buffer;
	istream_source_mgr.skip_input_data = &istream_source::skip_input_data;
	istream_source_mgr.resync_to_restart = &jpeg_resync_to_restart;
	istream_source_mgr.term_source = &istream_source::term_source;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	cinfo.src = &istream_source_mgr;

	(void) jpeg_read_header(&cinfo, TRUE);

	(void) jpeg_start_decompress(&cinfo);

	unsigned int row_stride = cinfo.output_width * cinfo.output_components;

	std::vector<unsigned char> raw_buffer(row_stride * cinfo.output_height);
	unsigned char* buffer[cinfo.output_height];
	for (unsigned int i=0; i < cinfo.output_height; ++i) buffer[i] = raw_buffer.data() + row_stride * i;

	while (cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, buffer + cinfo.output_scanline, cinfo.output_height - cinfo.output_scanline);
	}

	(void) jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);


	return surface(std::move(raw_buffer), cinfo.output_width, cinfo.output_height, cinfo.output_components);
}

}
