cmake_minimum_required(VERSION 2.6)

find_path(YMSE_PATH ymse/core.hpp $ENV{YMSE_PATH})
find_library(YMSE_LIBRARY ymse $ENV{YMSE_PATH}/src)

if (YMSE_PATH AND YMSE_LIBRARY)
	if (NOT Ymse_FIND_QUIETLY)
		message(STATUS "Located ymse includes at ${YMSE_PATH} and library at ${YMSE_LIBRARY}")
	endif (NOT Ymse_FIND_QUIETLY)
else (YMSE_PATH AND YMSE_LIBRARY)
	if (Ymse_FIND_REQUIRED)
		message(FATAL_ERROR
			"Could not find Ymse. Please set the environmental "
			"variable YMSE_PATH to the project path. This path "
			"should contain ymse/core.hpp and src/libymse.a"
		)
	endif (Ymse_FIND_REQUIRED)
endif (YMSE_PATH AND YMSE_LIBRARY)
