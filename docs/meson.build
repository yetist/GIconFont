conf = configuration_data()
conf.set('PACKAGE_VERSION', meson.project_version())
version_xml = configure_file(input: 'version.xml.in',
	output: 'version.xml',
	configuration: conf)

glib_prefix = dependency('glib-2.0').get_pkgconfig_variable('prefix')
glib_docpath = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')
docpath = join_paths(get_option('datadir'), 'gtk-doc', 'html')

gnome.gtkdoc(
	'gicon-font',
	main_xml : 'gicon-font.xml',
	src_dir: [
		join_paths(meson.source_root(), 'gicon-font'),
		join_paths(meson.build_root(), 'gicon-font'),
		],
	dependencies: libgicon_font_deps,
	gobject_typesfile: 'gicon-font.types',
	scan_args: ['--rebuild-types'],
	fixxref_args: [
		'--html-dir=@0@'.format(docpath),
		'--extra-dir=@0@'.format(join_paths(glib_docpath, 'glib')),
		'--extra-dir=@0@'.format(join_paths(glib_docpath, 'gobject')),
		'--extra-dir=@0@'.format(join_paths(glib_docpath, 'gio')),
		],
	install : true
	)
