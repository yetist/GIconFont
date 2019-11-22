gnome = import('gnome')
if builtin_fonts
  gawesome_resources = gnome.compile_resources(
    'gawesome-resource', 'gawesome.gresource.xml',
    source_dir: '../data',
    c_name: 'gawesome'
  )
endif

install_headers(
  'gawesome.h',
  subdir: 'gawesome',
)
if builtin_fonts
  source_list = [ 'gawesome.c', gawesome_resources]
else
  source_list = [ 'gawesome.c']
endif

libgawesome = shared_library('gawesome',
  source_list,
  dependencies : [freetype_dep, gtk_dep],
  install:true,
)
