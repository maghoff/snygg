#!/bin/bash

find src -name '*.cpp' -or -name '*.hpp' -or -name '*.glsl' | sort > snygg.files

echo CMakeLists.txt >> snygg.files
echo src/CMakeLists.txt >> snygg.files

echo README.txt >> snygg.files
