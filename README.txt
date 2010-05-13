Notes on how to build easily
============================

Preparation
-----------

Run ./prepare.sh to resolve most dependencies (on Ubuntu 10.04 at least)
and do the necessary build initialization (See CMake below)


Ymse
----

You need a checkout of ymse. From some proper directory, do this:

    hg clone http://bitbucket.org/maghoff/ymse/
    cd ymse
    cmake .
    make -j3


To let snygg locate ymse, please set the YMSE_PATH:

    export YMSE_PATH=`pwd`


Snygg
-----

Then build and run snygg (again, from the proper directory):

    make -Cdebug -j
    debug/src/snygg


CMake
-----

This project uses CMake. Prepare will do the necessary initial invocations of
CMake. After that, it is sufficient to run just make in the debug or release
directory.

