Build instructions
==================

Dependencies
------------

You need to supply the following libraries:

 * SDL
 * SDL_image
 * imlib2 (Optional, required for screenshots)
 * OpenGL

On Ubuntu, these should be resolved automatically by running ./prepare.sh


Configure
---------

	./waf configure

The configure step will let `waf` inspect the system and create the necessary
build configuration. It will also cause downloading of some additional
dependencies.

`waf` is a python script, so you can alternatively invoke it by calling the
interpreter explicitly. This is probably a good idea on Windows.


Build
-----

	./waf build_debug # and/or build_release


Run
---

	./build/debug/src/platform-desktop-sdl/snygg

Substitute `release` for `debug` in the above to use an optimized build.
