#!/bin/bash

PACKAGES="build-essential cmake freeglut3-dev libboost-dev libboost-filesystem-dev libboost-program-options-dev libsdl-dev libsdl-image1.2-dev curl libluabind-dev lintian desktop-file-utils"
CMAKE_PARAMS="-C../GCCSetup.cmake -DCMAKE_INSTALL_PREFIX=/usr -DLUABIND_MODE=system"
BUILD_TYPES="debug release"


if ! dpkg -l $PACKAGES >/dev/null
then
    echo "Will install packages: $PACKAGES"
    sudo apt-get install $PACKAGES
fi


./get_textures.sh


for x in $BUILD_TYPES
do
    mkdir -p "$x"

    pushd "$x"
    cmake $CMAKE_PARAMS "-DCMAKE_BUILD_TYPE=$x" ..
    popd
done

