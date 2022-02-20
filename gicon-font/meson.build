source_h = [
  'gicon-font.h',
]

if builtin_fonts
  gnome = import('gnome')
  giconfont_resources = gnome.compile_resources(
    'gicon-font-resource',
    'resource/giconfont.gresource.xml',
    source_dir: 'resource',
    c_name: 'gicon_font'
  )
  source_list = ['gicon-font.c', giconfont_resources]
else
  source_list = ['gicon-font.c']
endif

install_headers(
  source_h,
  subdir: 'gicon-font',
)

libgicon_font = shared_library('gicon-font',
  source_list,
  dependencies : [freetype_dep, gtk_dep],
  install:true,
)

libgicon_font_deps = declare_dependency(sources: source_list,
    include_directories: [top_srcdir, include_directories('.')],
    dependencies : [freetype_dep, gtk_dep],
    link_with: libgicon_font)

if get_option('introspection')
  libgicon_font_gir = gnome.generate_gir(libgicon_font,
    sources: [source_h, source_list],
    nsversion : api_version,
    namespace : 'GIconFont',
    symbol_prefix : 'gicon',
    identifier_prefix : 'GIcon',
    link_with: libgicon_font,
    includes : ['GObject-2.0', 'Gtk-3.0'],
    install : true,
    install_dir_gir: girdir,
    install_dir_typelib: typelibdir,
    extra_args: [ '--c-include=gicon-font/gicon-font.h' ],
    )
  if get_option('vapi')
    gicon_font_vapi = gnome.generate_vapi('gicon-font-' + api_version,
      sources: libgicon_font_gir.get(0),
      packages: ['gio-2.0', 'gtk+-3.0'],
      install: true,
      install_dir: vapidir,
      )
  endif
endif
