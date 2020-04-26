project('gawesome', 'c',
version : '0.1.0',
license : 'LGPLv2.1+',
default_options : [
	'warning_level=1',
	'buildtype=debugoptimized',
],
meson_version : '>=0.40.1'
)

# Compiler & Project arguments
add_project_arguments([
  '-DHAVE_CONFIG_H',
  '-I' + meson.build_root(),
  ],
  language: 'c'
)

freetype_dep = dependency('freetype2')
gtk_dep = dependency('gtk+-3.0')

top_srcdir = include_directories('.')
top_libdir = include_directories('./gawesome')

prefix = get_option('prefix')
datadir = join_paths(prefix, get_option('datadir'))
pkgdatadir = join_paths(datadir, meson.project_name())

conf = configuration_data()
conf.set_quoted('PACKAGE', meson.project_name())
conf.set_quoted('VERSION', meson.project_version())

conf.set_quoted('PACKAGE_NAME', meson.project_name())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())
conf.set_quoted('GETTEXT_PACKAGE', meson.project_name())
conf.set_quoted('DATADIR', pkgdatadir)

builtin_fonts = get_option('builtin_fonts')
if builtin_fonts
  conf.set('BUILT_IN_FONTS', 1)
endif

configure_file(
  output : 'config.h',
  configuration : conf
)

subdir('gawesome')
subdir('example')

pkg = import('pkgconfig')
pkg.generate(
	name: 'gawesome',
	version: meson.project_version(),
	description: 'Gobject awesome font library',
	filebase: 'gawesome',
	requires: ['gobject-2.0', 'awesome'],
	libraries: '-lgawesome',
	subdirs: 'gawesome',
)
