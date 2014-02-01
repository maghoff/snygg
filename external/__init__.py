#!/usr/bin/env python

import os, patch, zipfile, tarfile


class Exists:
	def __init__(self, path):
		self.path = path

	def __call__(self, installpath):
		return os.path.exists(os.path.join(installpath, self.path))

def zip(fileobj):
	return zipfile.ZipFile(fileobj, 'r')

def tar(fileobj):
	return tarfile.open(fileobj=fileobj, mode='r')


LUA = {
	"name": "lua",
	"uri": "http://www.lua.org/ftp/lua-5.1.5.tar.gz",
	"compression": tar,
	"is_installed": Exists("lua-5.1.5"),
	"includes": "lua-5.1.5",
}

LUABIND = {
	"name": "luabind",
	"uri": "http://sourceforge.net/projects/luabind/files/luabind/0.9.1/luabind-0.9.1.zip/download",
	"compression": zip,
	"is_installed": Exists("luabind-0.9.1"),
	"includes": "luabind-0.9.1",
	"patches": [
		"luabind-gcc-bug.patch",
	],
}

BOOST = {
	"name": "boost",
	"uri": "http://downloads.sourceforge.net/project/boost/boost/1.55.0/boost_1_55_0.zip",
	"compression": zip,
	"is_installed": Exists("boost_1_55_0"),
	"includes": "boost_1_55_0",
}

GLEW = {
	"name": "glew",
	"uri": "https://sourceforge.net/projects/glew/files/glew/1.5.8/glew-1.5.8.zip/download",
	"compression": zip,
	"is_installed": Exists("glew-1.5.8"),
	"includes": "glew-1.5.8/include",
}

JPEG = {
	"name": "jpeg",
	"uri": "http://downloads.sourceforge.net/project/libjpeg/libjpeg/6b/jpegsr6.zip",
	"compression": zip,
	"is_installed": Exists("jpeg-6b"),
	"includes": "jpeg-6b",
	"patches": [
		"jpeg6b.patch",
	],
}

PACKAGES = [
	LUA,
	LUABIND,
	BOOST,
	GLEW,
	JPEG,
]


def extract_uri(uri, compression, target_path, verbosity=1):
	import urllib2, StringIO

	if verbosity >= 1: print "Downloading %s..." % uri
	f = urllib2.urlopen(uri)
	b = StringIO.StringIO(f.read())
	if verbosity >= 1: print "Extracting...",
	z = compression(b)
	z.extractall(target_path)
	if verbosity >= 1: print "done"


def do_import(verbosity=1):
	basepath = os.path.join(os.path.dirname(__file__), "downloads")

	for p in PACKAGES:
		installpath = os.path.join(basepath, "")
		is_installed = p["is_installed"](installpath)

		if is_installed:
			if verbosity >= 1: print "%s already installed" % p["name"]
		else:
			extract_uri(p["uri"], p["compression"], installpath, verbosity)
			for patchFile in (p.get("patches", [])):
				if verbosity >= 1: print "Applying patch %s" % patchFile
				x = patch.fromfile(os.path.join(os.path.abspath(os.path.dirname(__file__)), patchFile))
				good = x.apply(1, os.path.join(os.path.abspath(os.path.dirname(__file__)), "downloads"))
				if not good:
					raise Exception("Failed to apply patch %s" % patchFile)


def configure(debug_env, release_env):
	basepath = os.path.join(os.path.abspath(os.path.dirname(__file__)), "downloads")

	debug_env.append_unique('CXXDEFINES', 'GLEW_STATIC')
	debug_env.append_unique('CCDEFINES', 'GLEW_STATIC')
	release_env.append_unique('CXXDEFINES', 'GLEW_STATIC')
	release_env.append_unique('CCDEFINES', 'GLEW_STATIC')

	for package in PACKAGES:
		includes_property_name = 'INCLUDES_%s' % package['name']
		includepath = os.path.join(basepath, package['includes'])
		setattr(debug_env, includes_property_name, includepath)
		setattr(release_env, includes_property_name, includepath)


if __name__ == '__main__':
	do_import()
