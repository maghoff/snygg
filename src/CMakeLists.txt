add_executable(
	snygg
	main.cpp
	snygg.cpp
)

target_link_libraries(
	snygg
	GL
	glut
	ymse
)
