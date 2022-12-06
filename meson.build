project('giconfont', ['c', 'vala'],
  version : '0.1.0',
  license : 'LGPLv2.1+',
  default_options : [
      'warning_level=1',
      'buildtype=debugoptimized',
      ],
  meson_version : '>=0.56.0'
)

api_version = '1.0'

# Compiler & Project arguments
add_project_arguments([
  '-DHAVE_CONFIG_H',
  '-I' + meson.current_build_dir(),
  ],
  language: 'c'
)

freetype_dep = dependency('freetype2')
gtk_dep = dependency('gtk+-3.0')

top_srcdir = include_directories('.')
top_libdir = include_directories('./giconfont')

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

if get_option('builtin_fonts')
  conf.set('BUILT_IN_FONTS', 1)
endif

configure_file(
  output : 'config.h',
  configuration : conf
)

# Setup various paths that subdirectory meson.build files need
package_subdir = get_option('package_subdir')
girdir = join_paths(get_option('datadir'), package_subdir, 'gir-1.0')
typelibdir = join_paths(get_option('libdir'), package_subdir, 'girepository-1.0')
libdir = join_paths(get_option('libdir'), package_subdir)
if package_subdir != ''
  vapidir = join_paths(get_option('datadir'), package_subdir, 'vapi')
else
  vapidir = join_paths(get_option('datadir'), 'vala', 'vapi')
endif

subdir('giconfont')
subdir('example')
if get_option('docs')
  subdir('docs')
endif
