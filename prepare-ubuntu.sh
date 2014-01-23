#!/bin/bash

PACKAGES="build-essential libsdl-dev libsdl-image1.2-dev lintian desktop-file-utils libimlib2-dev"

if ! dpkg -l $PACKAGES >/dev/null
then
    echo "Will install packages: $PACKAGES"
    sudo apt-get install $PACKAGES
fi
