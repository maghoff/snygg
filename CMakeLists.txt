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

set(LUABIND_INCLUDE_DIRS import/luabind-0.9)
set(LUABIND_LIBRARIES luabind)

include_directories(
	${YMSE_PATH}
	${SDL_INCLUDE_DIR}
	${LUA_INCLUDE_DIRS}
	${LUABIND_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

link_directories(
	${Boost_LIBRARY_DIRS}
	${LUA_LIBRARY_DIRS}
)

add_subdirectory(import)
add_subdirectory(src)

configure_file(
	install.sh.in
	install.sh
	@ONLY
)

