source_h = [
  'gicon-font.h',
]

install_headers(
  source_h,
  subdir: 'gicon-font',
)

if get_option('builtin_fonts')
  gnome = import('gnome')
  giconfont_resources = gnome.compile_resources(
    'gicon-font-resource',
    'resource/giconfont.gresource.xml',
    source_dir: 'resource',
    c_name: 'gicon_font'
  )
  source_c = ['gicon-font.c', giconfont_resources]
else
  source_c = ['gicon-font.c']
endif

libgicon_font = shared_library('gicon-font',
  source_c,
  dependencies : [freetype_dep, gtk_dep],
  install:true,
)

libgicon_font_deps = declare_dependency(
  sources: source_c,
  include_directories: [top_srcdir, include_directories('.')],
  dependencies : [freetype_dep, gtk_dep],
  link_with: libgicon_font
)

pkg = import('pkgconfig')
pkg.generate(
  name: 'gicon-font',
  version: meson.project_version(),
  description: 'Gobject icon font library',
  filebase: 'gicon-font',
  requires: ['freetype2', 'gtk+-3.0', 'gicon-font'],
  libraries: '-lgicon-font',
  subdirs: 'gicon-font',
)

if get_option('introspection')
  libgicon_font_gir = gnome.generate_gir(libgicon_font,
    sources: [source_h, source_c],
    nsversion : api_version,
    namespace : 'GIconFont',
    symbol_prefix : 'gicon',
    identifier_prefix : 'GIcon',
    link_with: libgicon_font,
    includes : ['Gtk-3.0'],
    install : true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
    extra_args: [ '--c-include=gicon-font/gicon-font.h' ],
    )
  if get_option('vapi')
    gicon_font_vapi = gnome.generate_vapi('gicon-font-' + api_version,
      sources: libgicon_font_gir.get(0),
      packages: ['gtk+-3.0'],
      install: true,
      install_dir: vapidir,
      )
  endif
endif
