include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(
	snygg MACOSX_BUNDLE
	arc.cpp
	ball_insert_iterator.cpp
	blood_pool.cpp
	board.cpp
	complex_polygon.cpp
	complex_polygon_triangulator.cpp
	contour_segment.cpp
	digesting_growth_policy.cpp
	extender.cpp
	food_generator.cpp
	food_item.cpp
	gl_fbo.cpp
	growth_policy.cpp
	immediate_growth_policy.cpp
	intersectable_with_circle.cpp
	item.cpp
	item_container.cpp
	item_with_life.cpp
	line.cpp
	luamod/lua_vm.cpp
	main.cpp
	metaballs.cpp
	open_segment.cpp
	paths.cpp
	plain_skin.cpp
	player.cpp
	renderable.cpp
	reverse_contour_segment.cpp
	scalable_skin.cpp
	schematic_skin.cpp
	scored_point.cpp
	segment_filter.cpp
	segment_sequence.cpp
	skin.cpp
	snake.cpp
	snake_direction_listener.cpp
	snygg.cpp
	textured_skin.cpp

	triangulation/construct.c
	triangulation/misc.c
	triangulation/monotone.c
	triangulation/tri.c
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/config.hpp
	ESCAPE_QUOTES
)

target_link_libraries(
	snygg
	${OPENGL_gl_LIBRARY}
	${OPENGL_glu_LIBRARY}
	${YMSE_LIBRARY}
	${SDLIMAGE_LIBRARY}
	${SDLMAIN_LIBRARY}
	${SDL_LIBRARY}
	${LUABIND_LIBRARIES}
	${LUA_LIBRARIES}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${COREFOUNDATION_LIBRARY}
	Imlib2 # Stupid CMake. ${IMLIB2_LIBRARY}
)

set_target_properties(
	snygg
	PROPERTIES

	COMPILE_DEFINITIONS "GL_GLEXT_PROTOTYPES"#;DISALLOW_LONG_PLAYING"
)

# Stupid CMake does not make it easy to put resources into a bundle
if (APPLE)
	add_custom_target(
		osx_bundle_dirs
		COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/snygg.app/Contents/Resources/levels
		COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/../levels/*.lua ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/snygg.app/Contents/Resources/levels/
		COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/../skins ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/snygg.app/Contents/Resources/
	)
	add_dependencies(snygg osx_bundle_dirs)
endif (APPLE)
