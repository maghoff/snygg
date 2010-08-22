cmake_minimum_required(VERSION 2.6)
project(snygg)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
find_package(Ymse REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(OpenGL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(LUA REQUIRED lua5.1 lua)

if (APPLE)
	find_library(COREFOUNDATION_LIBRARY CoreFoundation REQUIRED)
	find_path(COREFOUNDATION_INCLUDE_DIRS CFBundle.h)
else (APPLE)
	set(COREFOUNDATION_LIBRARY "")
	set(COREFOUNDATION_INCLUDE_DIRS "")
endif (APPLE)

include_directories(
	${YMSE_PATH}
	${SDL_INCLUDE_DIR}
	${LUA_INCLUDE_DIRS}
	${LUABIND_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	${COREFOUNDATION_INCLUDE_DIRS}
)

link_directories(
	${Boost_LIBRARY_DIRS}
	${LUA_LIBRARY_DIRS}
)

if ("${LUABIND_MODE}" STREQUAL "localbuild")
	add_subdirectory(import)
	set(LUABIND_INCLUDE_DIRS import/luabind-0.9)
	set(LUABIND_LIBRARIES luabind)
else ("${LUABIND_MODE}" STREQUAL "localbuild")
	pkg_check_modules(LUABIND REQUIRED luabind)
endif ("${LUABIND_MODE}" STREQUAL "localbuild")

include_directories(
	${LUABIND_INCLUDE_DIRS}
)

add_subdirectory(src)

configure_file(
	install.sh.in
	install.sh
	@ONLY
)

