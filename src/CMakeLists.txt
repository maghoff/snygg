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

target_link_libraries(
	snygg
	GL
	GLU
	glut
	${YMSE_LIBRARY}
	${SDLIMAGE_LIBRARY}
	${SDL_LIBRARY}
	${LUABIND_LIBRARIES}
	${LUA_LIBRARIES}
	${Boost_PROGRAM_OPTIONS_LIBRARY}
)

set_target_properties(
	snygg
	PROPERTIES

	COMPILE_FLAGS "-ggdb -ffast-math -pipe -fvisibility-inlines-hidden -Wall"
	COMPILE_DEFINITIONS "DEBUG;EBUG;GL_GLEXT_PROTOTYPES"#;DISALLOW_LONG_PLAYING"

#	COMPILE_FLAGS "-O3 -ffast-math -pipe -fvisibility-inlines-hidden"
#	COMPILE_DEFINITIONS "NDEBUG;GL_GLEXT_PROTOTYPES"
#	LINK_FLAGS "-s"
)

