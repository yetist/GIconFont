all_sources = [
  'main.c',
]

foreach src : all_sources
  p = src.split('.')
  exe = executable(p[0],
    sources: src,
    dependencies : [freetype_dep, gtk_dep],
    link_with : [libgicon_font],
    include_directories : [
      top_srcdir,
      top_libdir,
      ],
    install : false,
    )
  test(p[0], exe)
endforeach
