#include "gl.h"
#include <fstream>
#include <limits>
#include <stdexcept>
#include "fileutil.hpp"
#include "gl/shader.hpp"

namespace ymse {
namespace gl {

shader::shader(unsigned type) {
	id = glCreateShader(type);
}

shader::~shader() {
	glDeleteShader(id);
}

void shader::source_file(const std::string& filename) {
	std::vector<char> buffer;
	ymse::read_entire_file(buffer, filename);

	const char* string[] = { buffer.data() };
	std::size_t len = buffer.size();
	if (len > static_cast<std::size_t>(std::numeric_limits<int>::max())) throw std::runtime_error("Buffer too large");
	const int length[] = { static_cast<int>(len) };

	glShaderSource(id, 1, string, length);
	//check_for_gl_error();

	glCompileShader(id);
	//check_for_gl_error();
}

unsigned shader::get_id() const {
	return id;
}

}}
