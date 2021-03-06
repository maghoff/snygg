#!/bin/env python
# -*- coding: utf-8 -*-


DOWNLOAD_FILES = {
	'skins/snakeskin/diffuse.jpg': 'http://bitbucket.org/maghoff/snygg/downloads/diffuse.jpg',
	'skins/snakeskin/normal.jpg':  'http://bitbucket.org/maghoff/snygg/downloads/normal.jpg',
}


def download(files):
	import urllib2, os
	for local, remote in files.iteritems():
		if os.path.isfile(local): continue
		print("Downloading %s -> %s" % (remote, local))
		r = urllib2.urlopen(remote)
		data = r.read()
		with open(local, 'wb') as l: l.write(data)


def options(opt):
	opt.load('compiler_cxx')
	opt.load('compiler_c')

	opt.add_option('--sdl', dest='sdl_root', default=None, action='store', help='The path that contains include/SDL.h')
	opt.add_option('--ppapi', dest='ppapi_root', default=None, action='store', help='The path named pepper_nn')

	opt.add_option('--disallow-long-playing', dest='disallow_long_playing', default=False, action='store_true', help='If specified, the game will terminate when you score 3 points. Use this to focus on programming.')


def configure(conf):
	import os, sys, external, wafutil

	wafutil.msvc_initial_setup(conf.env, msvc_versions=[(10, 0)])

	conf.load('compiler_cxx')
	conf.load('compiler_c')

	conf.define('INSTALL_PREFIX', '/usr/games')

	cc = wafutil.get_compiler_configurator(conf)

	cc.sane_default(conf.env)
	cc.enable_cpp11(conf.env)
	cc.many_warnings(conf.env)
	cc.warnings_as_errors(conf.env)

	core_env = conf.env
	core_env.append_unique('DEFINES', 'M_PI=3.14159265358979323848')

	if conf.env['CXX_NAME'] == 'gcc':
		# Probably gcc or clang
		if any('clang' in x for x in conf.env['CXX']):
			# Probably clang
			core_env.append_unique('CXXFLAGS', '-Wno-error=overloaded-virtual')
			core_env.append_unique('CXXFLAGS', '-Wno-error=unused-function')
			# Instantiation of vec2f::z() is out of bounds
			core_env.append_unique('CXXFLAGS', '-Wno-error=array-bounds')

	from waflib.Options import options as opt

	if opt.sdl_root == None:
		conf.check_cfg(package='sdl', uselib_store='SDL', args=['--cflags', '--libs'])
	else:
		core_env.INCLUDES_SDL = [ os.path.join(opt.sdl_root, 'include') ]
		core_env.LIBPATH_SDL = [ os.path.join(opt.sdl_root, 'lib') ]
		core_env.LIB_SDL = [ 'SDL' ]

	core_env.LIB_SDLmain = [ 'SDLmain' ]

	conf.check_cfg(package='imlib2', uselib_store='imlib', args=['--cflags', '--libs'], mandatory=False)

	if opt.disallow_long_playing:
		conf.define('DISALLOW_LONG_PLAYING', 1)

	conf.write_config_header('../src/config.hpp', top=True)

	debug_env = core_env.derive()
	cc.debug_mode(debug_env)
	conf.setenv('debug', env = debug_env)

	release_env = core_env.derive()
	cc.release_mode(release_env)
	cc.optimize(release_env)
	cc.link_time_code_generation(release_env)
	conf.setenv('release', env = release_env)

	if opt.ppapi_root != None:
		includedirs = [ os.path.join(opt.ppapi_root, 'include') ]
		debuglibdirs = [ os.path.join(opt.ppapi_root, 'lib', 'pnacl', 'Debug') ]
		releaselibdirs = [ os.path.join(opt.ppapi_root, 'lib', 'pnacl', 'Release') ]

		def thing(name):
			setattr(debug_env, "INCLUDES_" + name, includedirs)
			setattr(release_env, "INCLUDES_" + name, includedirs)
			setattr(debug_env, "LIBPATH_" + name, debuglibdirs)
			setattr(release_env, "LIBPATH_" + name, releaselibdirs)
			setattr(debug_env, "LIB_" + name, [ name ])
			setattr(release_env, "LIB_" + name, [ name ])

		thing('ppapi_simple')
		thing('ppapi_gles2')
		thing('ppapi_cpp')
		thing('nacl_io')
		thing('ppapi')

		debug_env.CXXFLAGS_pthread = [ '-pthread' ]
		release_env.CXXFLAGS_pthread = [ '-pthread' ]
		debug_env.CCFLAGS_pthread = [ '-pthread' ]
		release_env.CCFLAGS_pthread = [ '-pthread' ]

	wafutil.configure_gl(debug_env, release_env)

	download(DOWNLOAD_FILES)

	external.do_import()
	external.configure(debug_env, release_env)


def core_build(bld):
	bld.recurse([
		'external',
		'src',
	])


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
