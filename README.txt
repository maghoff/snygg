Notes on how to build easily
============================

Dependencies
------------

Assuming you are on Ubuntu:

    sudo apt-get install build-essential cmake freeglut3-dev libboost-dev

(Please add more packages as you discover needs)


Ymse
----

You need a checkout of ymse. From some proper directory, do this:

    hg clone http://munkeliv.ath.cx/~mag/hg/ymse
    cd ymse
    cmake .
    make -j3


To let snygg locate ymse, please set the YMSE_PATH:

    export YMSE_PATH=`pwd`


Snygg
-----

Then build snygg (again, from the proper directory):

    cmake .
    make -j3

    ./src/snygg


CMake
-----

This project uses CMake. It is only necessary to run CMake the first time
you are going to build. After that, it is sufficient to run just make (or
the proper command for your system)

