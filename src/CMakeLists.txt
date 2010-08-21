include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(
	snygg
	arc.cpp
	blood_pool.cpp
	board.cpp
	digesting_growth_policy.cpp
	extender.cpp
	food_generator.cpp
	food_item.cpp
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
	scalable_skin.cpp
	scored_point.cpp
	segment_sequence.cpp
	skin.cpp
	snake.cpp
	snake_direction_listener.cpp
	snygg.cpp
	textured_skin.cpp
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
	${SDL_LIBRARY}
	${LUABIND_LIBRARIES}
	${LUA_LIBRARIES}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
)

set_target_properties(
	snygg
	PROPERTIES

	COMPILE_DEFINITIONS "GL_GLEXT_PROTOTYPES"#;DISALLOW_LONG_PLAYING"
)

