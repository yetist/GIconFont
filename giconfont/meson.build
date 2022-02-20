pub_source_h = [
  'giconfont.h',
]

pub_source_c = [
    'giconfont.c'
]

install_headers(
  pub_source_h,
  subdir: 'giconfont',
)

source_c = pub_source_c

if get_option('builtin_fonts')
  gnome = import('gnome')
  giconfont_resources = gnome.compile_resources(
    'giconfont-resource',
    'resource/giconfont.gresource.xml',
    source_dir: 'resource',
    c_name: 'gicon_font'
  )
  source_c += [giconfont_resources]
endif

libgicon_font = shared_library('giconfont-' + api_version,
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
  name: 'giconfont',
  version: meson.project_version(),
  description: 'Gobject icon font library',
  filebase: 'giconfont-' + api_version,
  #requires: ['freetype2', 'gtk+-3.0', 'giconfont'],
  libraries: ['-lgiconfont-1.0'],
  libraries_private: ['freetype2', 'gtk+-3.0'],
  subdirs: 'giconfont',
)

if get_option('introspection')
  libgicon_font_gir = gnome.generate_gir(libgicon_font,
    sources: [pub_source_h, pub_source_c],
    nsversion : api_version,
    namespace : 'GIconFont',
    symbol_prefix : 'gicon',
    identifier_prefix : 'GIcon',
    link_with: libgicon_font,
    includes : ['Gtk-3.0'],
    install : true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
    extra_args: [ '--c-include=giconfont/giconfont.h' ],
    )

  if get_option('vapi')
    gicon_font_vapi = gnome.generate_vapi('giconfont-' + api_version,
      sources: libgicon_font_gir.get(0),
      packages: ['gtk+-3.0'],
      install: true,
      install_dir: vapidir,
      )
  endif
endif
