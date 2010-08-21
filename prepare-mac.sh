#!/bin/bash

./get_textures.sh

pushd import
./dl.sh
popd

# We need to specify a CMAKE_BUILD_TYPE, but the resulting xcode-project can
# somehow be built in both Debug and Release modes with the correct flags.

mkdir -p xcode
pushd xcode
cmake \
	-GXcode \
	-C../GCCSetup.cmake \
	-DCMAKE_INSTALL_PREFIX=/usr \
	-DCMAKE_BUILD_TYPE=debug \
	-DCMAKE_OSX_ARCHITECTURES=x86_64 \
	-DLUABIND_MODE=localbuild \
	-DUSE_SDLMAIN=macosx \
	..
popd
