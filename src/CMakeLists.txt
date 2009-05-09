add_executable(
	snygg
	main.cpp
	plain_skin.cpp
	skin.cpp
	snygg.cpp
)

target_link_libraries(
	snygg
	GL
	glut
	ymse
)
