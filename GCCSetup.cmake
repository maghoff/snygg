# -D_GLIBCXX_DEBUG makes it crash :(
SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb -DEBUG -DDEBUG -ffast-math -pipe -Wall" CACHE STRING "LOL" FORCE)
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -ffast-math -pipe -Wall -Wl,-s" CACHE STRING "LOL" FORCE)

