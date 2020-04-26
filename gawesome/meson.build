if builtin_fonts
  gnome = import('gnome')
  gawesome_resources = gnome.compile_resources(
    'gawesome-resource', 'resource/gawesome.gresource.xml',
    source_dir: 'resource',
    c_name: 'gawesome'
  )
  source_list = ['gawesome.c', gawesome_resources]
else
  source_list = ['gawesome.c']
endif

install_headers(
  'gawesome.h',
  subdir: 'gawesome',
)

libgawesome = shared_library('gawesome',
  source_list,
  dependencies : [freetype_dep, gtk_dep],
  install:true,
)
