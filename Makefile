all:gawesome-resources.c gawesome-resources.h main.c
	gcc -o test $^ `pkg-config --cflags --libs freetype2 cairo gio-2.0`

gawesome-resources.c: gawesome.gresource.xml $(shell glib-compile-resources --sourcedir=. --generate-dependencies gawesome.gresource.xml)
	glib-compile-resources --target=$@ --sourcedir=$(srcdir) --generate --c-name gawesome $<

gawesome-resources.h: gawesome.gresource.xml $(shell glib-compile-resources --sourcedir=. --generate-dependencies gawesome.gresource.xml)
	glib-compile-resources --target=$@ --sourcedir=$(srcdir) --generate --c-name gawesome $<
