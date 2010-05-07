#!/bin/bash

PACKAGE=snygg
VER=0.0-1
SECTION=games
PRIORITY=optional
MAINTAINER="Magnus Hoff <email@example.com>"
DESCRIPTION="A snake-like game"

# To update this list, you might want to use:
#    objdump -p src/snygg | grep NEEDED | sed 's/ *NEEDED *//g' | xargs -n1 dpkg -S
DEPENDS="libgl1-mesa-glx, libglu1-mesa, libsdl-image1.2, libsdl1.2debian-alsa, libc6, libluabind0.9.0, liblua5.1-0, libboost-program-options1.40.0, libstdc++6, libgcc1"


function guess_architecture() {
    M="`uname -m`"
    case "$M" in
    x86_64) echo amd64 ;;
    *) echo "$M" ;;
    esac
}

ARCHITECTURE=`guess_architecture`


PKG_DIR="$PACKAGE-${VER}_$ARCHITECTURE"
PKG_NAME="$PKG_DIR.deb"

DEBIAN_DIR="$PKG_DIR/DEBIAN"
CONTROL="$DEBIAN_DIR/control"


rm -rf "$PKG_DIR"

mkdir -p "$PKG_DIR/usr/games"
mkdir -p "$DEBIAN_DIR"

# Copy files:
PREFIX="$PKG_DIR/usr" ./install.sh

# Create control-file:
echo "Package: $PACKAGE" > "$CONTROL"
echo "Version: $VER" >> "$CONTROL"
echo "Section: $SECTION" >> "$CONTROL"
echo "Priority: $PRIORITY" >> "$CONTROL"
echo "Architecture: $ARCHITECTURE" >> "$CONTROL"
echo "Depends: $DEPENDS" >> "$CONTROL"
echo "Maintainer: $MAINTAINER" >> "$CONTROL"
echo "Description: $DESCRIPTION" >> "$CONTROL"


fakeroot dpkg-deb --build "$PKG_DIR"

if ! lintian --fail-on-warnings "$PKG_NAME"
then
    echo "Lintian complained. Failing"
    exit 1
fi

