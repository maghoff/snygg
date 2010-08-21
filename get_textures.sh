#!/bin/bash

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
