#!/bin/bash

if ! [ "$PREFIX" ] ;
then
	PREFIX=/opt/snygg
fi

BIN="$PREFIX/games"
SHARE="$PREFIX/share/games/snygg"
LEVELS="$SHARE/levels"
SKINS="$SHARE/skins"

mkdir -p "$BIN" "$LEVELS" "$SKINS"

cp src/snygg "$BIN/"
cp levels/*.lua "$LEVELS/"
cp -r skins/* "$SKINS/"
