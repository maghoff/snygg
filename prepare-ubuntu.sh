#!/bin/bash

PACKAGES="build-essential freeglut3-dev libboost-dev libboost-filesystem-dev libboost-program-options-dev libsdl-dev libsdl-image1.2-dev curl libluabind-dev lintian desktop-file-utils libimlib2-dev"


if ! dpkg -l $PACKAGES >/dev/null
then
    echo "Will install packages: $PACKAGES"
    sudo apt-get install $PACKAGES
fi


./get_textures.sh


./waf configure
