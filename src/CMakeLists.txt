add_executable(
	snygg
	arc.cpp
	blood_pool.cpp
	board.cpp
	intersectable_with_circle.cpp
	line.cpp
	main.cpp
	plain_skin.cpp
	player.cpp
	renderable.cpp
	scored_point.cpp
	segment_sequence.cpp
	skin.cpp
	snake.cpp
	snygg.cpp
)

target_link_libraries(
	snygg
	GL
	glut
	${YMSE_LIBRARY}
	${SDL_LIBRARY}
)

set_target_properties(
    snygg
    PROPERTIES
    COMPILE_FLAGS "-ggdb -ffast-math -pipe -fvisibility-inlines-hidden -Wall"
    COMPILE_DEFINITIONS "DEBUG;EBUG"
#	COMPILE_FLAGS "-O3 -ffast-math -pipe -fvisibility-inlines-hidden"
#	COMPILE_DEFINITIONS "NDEBUG;GL_GLEXT_PROTOTYPES"
)
