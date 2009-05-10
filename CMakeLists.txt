cmake_minimum_required(VERSION 2.6)
project(SNYGG)

find_path(YMSE_PATH ymse/core.hpp $ENV{YMSE_PATH})
find_library(YMSE_LIBRARY ymse $ENV{YMSE_PATH}/src)

if (YMSE_PATH AND YMSE_LIBRARY)
	message(STATUS "Located ymse includes at ${YMSE_PATH} and library at ${YMSE_LIBRARY}")
else (YMSE_PATH AND YMSE_LIBRARY)
	message(FATAL_ERROR
		"Could not find Ymse. Please set the environmental "
		"variable YMSE_PATH to the project path. This path "
		"should contain ymse/core.hpp and src/libymse.a"
	)
endif (YMSE_PATH AND YMSE_LIBRARY)

include_directories(${YMSE_PATH})
add_subdirectory(src)
