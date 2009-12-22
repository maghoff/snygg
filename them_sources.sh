#!/bin/bash

find src skins levels -name '*.cpp' -or -name '*.hpp' -or -name '*.glsl' -or -name '*.lua' | sort > snygg.files

echo CMakeLists.txt >> snygg.files
echo src/CMakeLists.txt >> snygg.files

echo README.txt >> snygg.files
