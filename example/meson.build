all_sources = [
  'main.c',
]

giconfont_dep = dependency('giconfont-' + api_version)
exe = executable('browser',
  sources: 'browser.vala',
  dependencies : [gtk_dep, giconfont_dep],
  link_with : [libgicon_font],
  include_directories : [
  top_srcdir,
  top_libdir,
  ],
  vala_args: ['--pkg=giconfont-' + api_version, '--vapidir=' + meson.project_source_root() + '/giconfont'],
  install : false,
)

foreach src : all_sources
  p = src.split('.')
  exe = executable(p[0],
    sources: src,
    dependencies : [gtk_dep],
    link_with : [libgicon_font],
    include_directories : [
      top_srcdir,
      top_libdir,
      ],
    install : false,
    )
  test(p[0], exe)
endforeach
