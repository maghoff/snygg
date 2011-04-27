#!/bin/env python
# -*- coding: utf-8 -*-


def options(opt):
	opt.load('compiler_cxx')
	opt.load('compiler_c')

	opt.add_option('--sdl-includes', dest='sdl_includes', default=None, action='store', help='The path that contains SDL.h')
	opt.add_option('--sdl-lib', dest='sdl_lib', default=None, action='store', help='The SDL library')

	opt.add_option('--sdlmain-lib', dest='sdlmain_lib', default=None, action='store', help='The SDLmain library')

	opt.add_option('--sdl-image-includes', dest='sdl_image_includes', default=None, action='store', help='The path that contains SDL_image.h')
	opt.add_option('--sdl-image-lib', dest='sdl_image_lib', default=None, action='store', help='The SDL_image library')

	opt.add_option('--lua-includes', dest='lua_includes', default=None, action='store', help='The path that contains lua.h')
	opt.add_option('--lua-lib', dest='lua_lib', default=None, action='store', help='The LUA library')


def configure(conf):
	import os, sys, external
	try:
		sys.path.append(os.environ['YMSE_PATH'])
		import pymse.wafutil as wafutil
	except Exception, e:
		print "Failed to load pymse.wafutil. Bailing out"
		print e
		sys.exit(1)

	wafutil.msvc_initial_setup(conf.env, msvc_versions=[(10, 0)])

	conf.load('compiler_cxx')
	conf.load('compiler_c')

	cc = wafutil.get_compiler_configurator(conf)

	cc.sane_default(conf.env)
	cc.many_warnings(conf.env)
	#cc.warnings_as_errors(conf.env)

	core_env = conf.env.derive()

	from Options import options as opt

	if opt.sdl_includes != None:
		core_env.INCLUDES_SDL = [ opt.sdl_includes ]
	if opt.sdl_lib != None:
		core_env.LIBPATH_SDL = [ opt.sdl_lib ]
	core_env.LIB_SDL = [ 'SDL' ]

	if opt.sdlmain_lib != None:
		core_env.LIBPATH_SDLmain = [ opt.sdlmain_lib ]
	core_env.LIB_SDLmain = [ 'SDLmain' ]

	if opt.sdl_image_includes != None:
		core_env.INCLUDES_SDL_image = [ opt.sdl_image_includes ]
	if opt.sdl_image_lib != None:
		core_env.LIBPATH_SDL_image = [ opt.sdl_image_lib ]
	core_env.LIB_SDL_image = [ 'SDL_image' ]

	if opt.lua_includes != None:
		core_env.INCLUDES_lua = [ opt.lua_includes ]
	if opt.lua_lib != None:
		core_env.LIBPATH_lua = [ opt.lua_lib ]
	core_env.LIB_lua = [ 'lua5.1' ]

	debug_env = core_env.derive()
	cc.debug_mode(debug_env)
	conf.setenv('debug', env = debug_env)

	release_env = core_env.derive()
	cc.release_mode(release_env)
	cc.optimize(release_env)
	cc.link_time_code_generation(release_env)
	conf.setenv('release', env = release_env)

	wafutil.configure_ymse(debug_env, release_env)

	external.do_import()
	external.configure(debug_env, release_env)


def core_build(bld):
	bld.add_subdirs('src')


# All of the following boiler plate is recommended (demanded) by the waf book for enabling build variants:

def build(bld):
	if not bld.variant:
		bld.fatal('call "waf build_debug" or "waf build_release", and try "waf --help"')

	core_build(bld)

from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext

for x in ['debug', 'release']:
	for y in (BuildContext, CleanContext, InstallContext, UninstallContext):
		name = y.__name__.replace('Context','').lower()
		class tmp(y):
			cmd = name + '_' + x
			variant = x
