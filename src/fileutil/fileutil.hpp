#ifndef FILEUTIL_HPP
#define FILEUTIL_HPP

#include <iosfwd>
#include <string>
#include <vector>

void read_entire_file(std::vector<char>&, const std::string& filename);
void read_entire_stream(std::vector<char>&, std::istream& in);

#endif // FILEUTIL_HPP
