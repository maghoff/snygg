cmake_minimum_required(VERSION 2.6)
project(snygg)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")
find_package(Ymse REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA lua5.1 REQUIRED)
pkg_check_modules(LUABIND luabind REQUIRED)

include_directories(
	${YMSE_PATH}
	${SDL_INCLUDE_DIR}
	${LUABIND_INCLUDE_DIRS}
	${LUA_INCLUDE_DIRS}
)
add_subdirectory(src)
