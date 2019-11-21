/*
   * How to use?
   * edit source file, add the follow line:
   * #include <debug.h>
   * debug_print("%s", "debug here");
   *
 * */

#include <glib.h>

#define debug_print(fmt, ARGS...)  do { g_printerr (G_STRLOC ":" fmt "\n", ##ARGS); } while (0)

