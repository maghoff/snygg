cmake_minimum_required(VERSION 2.6)

if (POLICY CMP0011)
  cmake_policy(SET CMP0011 NEW)
endif (POLICY CMP0011)

find_path(YMSE_PATH ymse/core.hpp $ENV{YMSE_PATH})
find_library(YMSE_LIBRARY_DEBUG ymse $ENV{YMSE_PATH}/debug/src)
find_library(YMSE_LIBRARY_RELEASE ymse $ENV{YMSE_PATH}/release/src)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")
	set(YMSE_LIBRARY ${YMSE_LIBRARY_DEBUG})
endif ("${CMAKE_BUILD_TYPE}" STREQUAL "debug")

if (${CMAKE_BUILD_TYPE} STREQUAL "release")
	set(YMSE_LIBRARY ${YMSE_LIBRARY_RELEASE})
endif (${CMAKE_BUILD_TYPE} STREQUAL "release")

if (YMSE_PATH AND YMSE_LIBRARY)
	if (NOT Ymse_FIND_QUIETLY)
		message(STATUS "Located ymse includes at ${YMSE_PATH} and library at ${YMSE_LIBRARY}")
	endif (NOT Ymse_FIND_QUIETLY)
else (YMSE_PATH AND YMSE_LIBRARY)
	if (Ymse_FIND_REQUIRED)
		message(FATAL_ERROR
			"Could not find Ymse. Please set the environmental "
			"variable YMSE_PATH to the project path. This path "
			"should contain ymse/core.hpp, debug/src/libymse.a "
			"and release/src/libymse.a"
		)
	endif (Ymse_FIND_REQUIRED)
endif (YMSE_PATH AND YMSE_LIBRARY)

