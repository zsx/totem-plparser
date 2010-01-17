# vim: ft=python expandtab

import os
from site_init import GBuilder, Initialize

opts = Variables()
opts.Add(PathVariable('PREFIX', 'Installation prefix', os.path.expanduser('~/FOSS'), PathVariable.PathIsDirCreate))
opts.Add(BoolVariable('DEBUG', 'Build with Debugging information', 0))
opts.Add(PathVariable('PERL', 'Path to the executable perl', r'C:\Perl\bin\perl.exe', PathVariable.PathIsFile))

env = Environment(variables = opts,
                  ENV=os.environ, tools = ['default', GBuilder])

Initialize(env)

TOTEM_PL_PARSER_VERSION_MAJOR=2
TOTEM_PL_PARSER_VERSION_MINOR=29
TOTEM_PL_PARSER_VERSION_RELEASE=1
TOTEM_PL_PARSER_VERSION_STRING="%d.%d.%d" % (TOTEM_PL_PARSER_VERSION_MAJOR, TOTEM_PL_PARSER_VERSION_MINOR, TOTEM_PL_PARSER_VERSION_RELEASE)

env['DOT_IN_SUBS'] = {'@PACKAGE_VERSION@': TOTEM_PL_PARSER_VERSION_STRING,
					  '@VERSION@': TOTEM_PL_PARSER_VERSION_STRING,
					  '@TOTEM_PL_PARSER_VERSION_MAJOR@': str(TOTEM_PL_PARSER_VERSION_MAJOR),
					  '@TOTEM_PL_PARSER_VERSION_MINOR@': str(TOTEM_PL_PARSER_VERSION_MINOR),
					  '@TOTEM_PL_PARSER_VERSION_RELEASE@': str(TOTEM_PL_PARSER_VERSION_RELEASE),
					  '@prefix@': env['PREFIX'],
 					  '@exec_prefix@': '${prefix}/bin',
					  '@libdir@': '${prefix}/lib',
					  '@includedir@': '${prefix}/include'}
# Requirements
GLIB_REQS="2.21.6"
GIO_REQS="2.17.3"
DBUS_REQS="0.61"

env.ParseConfig('pkg-config glib-2.0 --atleast-version=%s --cflags --libs'%GLIB_REQS)
env.ParseConfig('pkg-config gio-2.0 --atleast-version=%s --cflags --libs'%GIO_REQS)
env.ParseConfig('pkg-config libxml-2.0 --cflags --libs')
env.ParseConfig('pkg-config gthread-2.0 --cflags --libs')
#env.ParseConfig('pkg-config dbus --atleast-version=%s --cflags --libs'%GLIB_REQS)
env.Append(CPPPATH=['#'])
env.Append(CFLAGS=env['DEBUG_CFLAGS'])
env.Append(CPPDEFINES=env['DEBUG_CPPDEFINES'])

env.DotIn('totem-plparser.pc', 'totem-plparser.pc.in')
env.DotIn('totem-plparser.rc', 'totem-plparser.rc.in')
env.DotIn('totem-plparser-mini.pc', 'totem-plparser-mini.pc.in')
env.DotIn('totem-plparser-mini.rc', 'totem-plparser-mini.rc.in')

env.DotIn('config.h', 'config.h.win32.in')
env.Command('config.h.win32.in', ['config.h.in', 'config.h.in.win32.patch'], '''    copy /y config.h.in config.h.win32.in
    patch < config.h.in.win32.patch''')

SConscript(['lib/SConscript', 'plparse/SConscript', 'plparse/tests/SConscript'], exports='env')


'''
name = 'png%s%s' % (TOTEM_PL_PARSER_VERSION_MAJOR, TOTEM_PL_PARSER_VERSION_MINOR)
libname = 'lib' + name

env.DotIn('scripts/' + name + '.pc', 'scripts/libpng.pc.in')
env.Alias('install', env.Install('$PREFIX/lib/pkgconfig', 'scripts/' + name + '.pc'))

libpng_SOURCES = Split("png.c pngset.c pngget.c pngrutil.c pngtrans.c pngwutil.c \
	pngread.c pngrio.c pngwio.c pngwrite.c pngrtran.c \
	pngwtran.c pngmem.c pngerror.c pngpread.c")

env.RES('scripts/pngw32.res', 'scripts/pngw32.rc')
dll = env.SharedLibrary([libname + env['LIB_SUFFIX'] + '.dll', name + '.lib'], libpng_SOURCES + ['scripts/pngw32.def', 'scripts/pngw32.res'])

env.AddPostAction(dll, 'mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2')

env.Alias('install', env.Install('$PREFIX/include/' + libname, ['png.h', 'pngconf.h']))
env.Alias('install', env.Install('$PREFIX/bin', libname + env['LIB_SUFFIX'] + '.dll'))
env.Alias('install', env.Install('$PREFIX/lib', name + '.lib'))
'''
