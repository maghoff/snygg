
# Unable to use -D_GLIBCXX_DEBUG due to other libraries that collide with this (ymse, perhaps?)
SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb -DEBUG -DDEBUG -ffast-math -pipe -Wall -Werror" CACHE STRING "LOL" FORCE)

SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -ffast-math -pipe -Wall -Werror -Wl,-s" CACHE STRING "LOL" FORCE)

