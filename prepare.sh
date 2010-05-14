#!/bin/bash

PACKAGES="build-essential cmake freeglut3-dev libboost-dev libboost-filesystem-dev libboost-program-options-dev libsdl-dev libsdl-image1.2-dev curl libluabind-dev lintian"
CMAKE_PARAMS="-C../GCCSetup.cmake -DCMAKE_INSTALL_PREFIX=/usr"
BUILD_TYPES="debug release"


if ! dpkg -l $PACKAGES >/dev/null
then
    echo "Will install packages: $PACKAGES"
    sudo apt-get install $PACKAGES
fi


function get() {
    TRG="$1"
    SRC="$2"
    if ! [ -f "$TRG" ]
    then
        curl -L -o "$TRG" "$SRC"
    fi
}

get skins/snakeskin/diffuse.jpg http://imgur.com/xKUVi.jpg
get skins/snakeskin/normal.jpg http://imgur.com/IkERt.jpg


for x in $BUILD_TYPES
do
    mkdir -p "$x"

    pushd "$x"
    cmake $CMAKE_PARAMS "-DCMAKE_BUILD_TYPE=$x" ..
    popd
done

