#!/bin/bash

function get() {
    TRG="$1"
    SRC="$2"
    if ! [ -f "$TRG" ]
    then
        curl -L -o "$TRG" "$SRC"
    fi
}

get skins/snakeskin/diffuse.jpg http://bitbucket.org/maghoff/snygg/downloads/diffuse.jpg
get skins/snakeskin/normal.jpg  http://bitbucket.org/maghoff/snygg/downloads/normal.jpg
