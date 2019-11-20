/* vi: set sw=4 ts=4 wrap ai: */
/*
 * main.c: This file is part of ____
 *
 * Copyright (C) 2019 Wu Xiaotian <yetist@gmail.com>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>
#include <cairo-ft.h>
#include <ft2build.h>

#include <gio/gio.h>

GBytes* load_resource(void)
{
    GResource *res;
    GError *error = NULL;
    GBytes *bytes;
    bytes = g_resources_lookup_data ("/cc/zhcn/gawesome/font.ttf",
            G_RESOURCE_LOOKUP_FLAGS_NONE,
            &error);
    if (error != NULL) {
        g_print("%s\n", error->message);
    }
    return bytes;
}

int main ()
{
    FT_Library value;
    FT_Error status;
    FT_Face face;

    cairo_t *c;
    cairo_surface_t *cs;
    cairo_font_face_t * ct;
    int size = 200;

    /* new code */
    GBytes *bytes;
    gsize byte_size;
    gconstpointer buffer;
    /* new code */

    bytes = load_resource();
    buffer = g_bytes_get_data (bytes, &byte_size);

    cs = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, size, size);
    c = cairo_create (cs);

    const char * filename = "wqy-microhei-lite.ttc";

    status = FT_Init_FreeType (& value);
    if (status != 0) {
        fprintf (stderr, "Error %d opening library.\n", status);
        exit (EXIT_FAILURE);
    }

    /* new code start */
    status = FT_New_Memory_Face(value,
                            buffer,    /* first byte in memory */
                            byte_size,      /* size in bytes        */
                            0,         /* face_index           */
                            &face);
    /* new code end */

    if (status != 0) {
        fprintf (stderr, "Error %d opening %s.\n", status, filename);
        exit (EXIT_FAILURE);
    }
    ct = cairo_ft_font_face_create_for_ft_face (face, 0);
    cairo_set_font_face (c, ct);
    cairo_set_font_size (c, size/3);
    cairo_set_source_rgb (c, 0, 0, 0);
    cairo_move_to (c, size/6, size/3);
    cairo_show_text (c, "あい");
    cairo_set_source_rgb (c, 0.5, 0.5, 0);
    cairo_move_to (c, size/6, 5*size/6);
    cairo_show_text (c, "金柑");
    cairo_surface_write_to_png (cs, "free-type.png");
    return 0;
}
