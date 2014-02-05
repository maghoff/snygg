#!/usr/bin/env python

def msvc_initial_setup(env, msvc_versions=[(10, 0)], msvc_targets=['x86']):
	env['MSVC_VERSIONS'] = ['msvc %i.%i' % x for x in msvc_versions]
	env['MSVC_TARGETS'] = msvc_targets

def configure_gl(debug_env, release_env):
	import platform
	if platform.system() == 'Darwin':
		frameworks = ['OpenGL', 'SDL', 'Cocoa']
		flags = sum((['-framework', x] for x in frameworks), [])
		release_env.LINKFLAGS_gl = flags
		debug_env.LINKFLAGS_gl = flags
	elif platform.system() == 'Linux':
		libs = ['GL', 'SDL']
		release_env.LIB_gl = libs
		debug_env.LIB_gl = libs
	elif platform.system() == 'Windows':
		libs = ['opengl32']
		release_env.LIB_gl = libs
		debug_env.LIB_gl = libs

def configure_external(debug_env, release_env):
	import os, imp
	basepath = os.path.join(os.path.dirname(__file__), "..")
	mod = imp.find_module('external', [basepath])
	external = imp.load_module('_avoid_name_collision_external', *mod)
	if mod[0] != None: mod[0].close()
	external.configure(debug_env, release_env)

def do_some_configuration(conf):
	"""Setup debug and build variants with some reasonable compiler flags.
	This will most certainly not be sufficient for all needs, but it will
	be for many. It is a good starting point."""

	msvc_initial_setup(conf.env)

	conf.load('compiler_cxx')

	cc = get_compiler_configurator(conf)

	cc.sane_default(conf.env)
	cc.many_warnings(conf.env)
	cc.warnings_as_errors(conf.env)

	core_env = conf.env.derive()

	debug_env = core_env.derive()
	cc.debug_mode(debug_env)
	conf.setenv('debug', env = debug_env)

	release_env = core_env.derive()
	cc.release_mode(release_env)
	cc.optimize(release_env)
	cc.link_time_code_generation(release_env)
	conf.setenv('release', env = release_env)

	configure_gl(debug_env, release_env)



class gcc_configurator:
	@staticmethod
	def sane_default(env):
		env.append_unique('CCFLAGS', '-fPIC')
		env.append_unique('CXXFLAGS', '-fPIC')
		env.append_unique('LINKFLAGS', '-fPIC')

	@staticmethod
	def debug_mode(env):
		env.append_unique('CXXFLAGS', '-ggdb')
		env.append_unique('CXXDEFINES', 'DEBUG')
		env.append_unique('CCFLAGS', '-ggdb')
		env.append_unique('CCDEFINES', 'DEBUG')
		env.append_unique('LINKFLAGS', '-rdynamic')

	@staticmethod
	def release_mode(env):
		env.append_unique('CXXDEFINES', 'NDEBUG')
		env.append_unique('CCDEFINES', 'NDEBUG')
		env.append_unique('LINKFLAGS', '-s')

	@staticmethod
	def optimize(env):
		env.append_unique('CXXFLAGS', '-O3')
		env.append_unique('CCFLAGS', '-O3')

	@staticmethod
	def many_warnings(env):
		env.append_unique('CXXFLAGS', '-Wall')
		env.append_unique('CCFLAGS', '-Wall')
		env.append_unique('LINKFLAGS', '-Wall')

	@staticmethod
	def warnings_as_errors(env):
		env.append_unique('CXXFLAGS', '-Werror')
		env.append_unique('CCFLAGS', '-Werror')
		env.append_unique('LINKFLAGS', '-Werror')

	@staticmethod
	def link_time_code_generation(env):
		if env['CC_VERSION'] >= ('4', '5'):
			env.append_unique('CXXFLAGS', '-flto')
			env.append_unique('CCFLAGS', '-flto')
			env.append_unique('LINKFLAGS', '-flto')
			# WHOPR, which is also available in GCC 4.5, is a more scalable
			# alternative, but it optimizes less. From GCC 4.6, WHOPR is the
			# default when specifying LTO like this.

	@staticmethod
	def enable_cpp11(env):
		env.append_unique('CXXFLAGS', '-std=c++11')


class msvc_configurator:
	@staticmethod
	def sane_default(env):
		env.append_unique('CXXFLAGS', '/FC') # Full path for error messages
		env.append_unique('CXXFLAGS', '/EHsc') # Enable Exceptions
		env.append_unique('CXXFLAGS', '/GR') # Enable RTTI
		env.append_unique('CXXFLAGS', '/GS') # Buffer Security Check
		env.append_unique('DEFINES', '_USE_MATH_DEFINES') # Enable standard behaviour for math.h (causes M_PI to be defined)
		env.append_unique('DEFINES', '_SCL_SECURE_NO_WARNINGS') # Disable warnings for secure iterators, to avoid compiler specific code
		env.append_unique('DEFINES', 'BOOST_ALL_NO_LIB')
		env.append_unique('LINKFLAGS', '/SUBSYSTEM:WINDOWS')
		#env.STLIB_ST = '%s.lib'
		#env.SHLIB_ST = '%s.lib'

	@staticmethod
	def debug_mode(env):
		env.append_unique('CXXFLAGS', '/Z7') # Debugging information
		env.append_unique('CXXFLAGS', '/MDd')
		env.append_unique('CXXFLAGS', '/Od')
		env.append_unique('CXXFLAGS', '/RTC1')
		env.append_unique('LINKFLAGS', '/DEBUG')
		env.append_unique('CXXDEFINES', 'DEBUG')
		env.append_unique('CCFLAGS', '/MDd')
		env.append_unique('CCDEFINES', 'DEBUG')

	@staticmethod
	def release_mode(env):
		env.append_unique('CXXFLAGS', '/MD')
		env.append_unique('CXXDEFINES', 'NDEBUG')
		env.append_unique('CCFLAGS', '/MD')
		env.append_unique('CCDEFINES', 'NDEBUG')

	@staticmethod
	def optimize(env):
		env.append_unique('CXXFLAGS', '/O2')
		env.append_unique('CCFLAGS', '/O2')

	@staticmethod
	def many_warnings(env):
		pass

	@staticmethod
	def warnings_as_errors(env):
		env.append_unique('CXXFLAGS', '/WX')
		env.append_unique('CCFLAGS', '/WX')
		env.append_unique('LINKFLAGS', '/WX')

	@staticmethod
	def link_time_code_generation(env):
		env.append_unique('CXXFLAGS', '/GL')
		env.append_unique('CCFLAGS', '/GL')
		env.append_unique('LINKFLAGS', '/LTCG')

	@staticmethod
	def enable_cpp11(env):
		# This is supposedly enabled by default
		pass


def get_compiler_configurator(conf):
	compiler_configurators = {
		'gcc': gcc_configurator,
		'msvc': msvc_configurator,
	}

	return compiler_configurators[conf.env['CXX_NAME']]
