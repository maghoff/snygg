#!/bin/bash


PACKAGE=snygg
VER=0.0-1
SECTION=games
PRIORITY=optional
ARCHITECTURE=AUTO_FURTHER_DOWN
DEPENDS=libc6
MAINTAINER="Magnus Hoff <email@example.com>"
DESCRIPTION="A snake-like game"


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
cp src/snygg "$PKG_DIR/usr/games/"

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

