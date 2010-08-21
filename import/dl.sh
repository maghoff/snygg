#!/bin/bash

SRC_URI='http://downloads.sourceforge.net/project/luabind/luabind/0.9/luabind-0.9.tar.gz?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fluabind%2Ffiles%2Fluabind%2F&ts=1282400796'

if ! [ -d luabind-0.9 ]
then
	curl -L "$SRC_URI" | tar zx
fi
