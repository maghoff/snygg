#ifndef PATHS_HPP
#define PATHS_HPP

#include <fstream>
#include <boost/filesystem/path.hpp>

namespace paths {

/// Store the value of argv[0]. This is used to guess where datafiles are
/// located.
void set_argv_zero(const char* argv_zero);

boost::filesystem::path levels(); //< data() / "levels"
boost::filesystem::path skins(); //< data() / "skins"

/// Opens the file *requested_path* either at its absolute position (if the
/// path is absolute), inside of *search_path*, or relative to the current
/// working directory. Returns the path where the file was found.
///
/// This is useful for paths input by the user.
boost::filesystem::path open_absolute_or_in_path(
	std::ifstream& in,
	const boost::filesystem::path& requested_path,
	const boost::filesystem::path& search_path
);

/// Like open_absolute_or_in_path above, but doesn't result in an open
/// ifstream. This, in turn, gives rise to a small race condition.
boost::filesystem::path find_absolute_or_in_path(
	const boost::filesystem::path& requested_path,
	const boost::filesystem::path& search_path
);

}

#endif // PATHS_HPP
