add_executable(
	snygg
	arc.cpp
	line.cpp
	main.cpp
	plain_skin.cpp
	segment.cpp
	skin.cpp
	snygg.cpp
)

target_link_libraries(
	snygg
	GL
	glut
	ymse
)
