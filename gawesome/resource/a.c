/* vi: set sw=4 ts=4 wrap ai: */
/*
 * a.c: This file is part of ____
 *
 * Copyright (C) 2019 yetist <yetist@yetibook>
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

#include <glib.h>

int main()
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GKeyFile) key_file = g_key_file_new ();

    if (!g_key_file_load_from_file (key_file, "fontawesome-webfont.map", G_KEY_FILE_NONE, &error))
    {
        if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
            g_warning ("Error loading key file: %s", error->message);
        return;
    }

    g_autofree gchar *val = g_key_file_get_string (key_file, "FontAwesome", "500px", &error);
    if (val == NULL &&
            !g_error_matches (error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND))
    {
        g_warning ("Error finding key in key file: %s", error->message);
        return;
    }
    else if (val == NULL)
    {
        val = g_strdup ("default-value");
    }
    g_print("val=%s\n", val);
    gunichar number = strtol(val, NULL, 0);

    //int number = (int)strtol(val, NULL, 0);
    g_print("number=%d\n", number);


    guint64 id;
    id = g_key_file_get_uint64 (key_file, "FontAwesome", "amazon", &error);
    g_print("%u\n", id);

    return 0;

#if 0
    /* load font code map */
    GBytes *map_bytes;
    map_bytes = g_resource_lookup_data (resource,
            "/cc/zhcn/gawesome/font.map",
            G_RESOURCE_LOOKUP_FLAGS_NONE,
            error);
    if (*error != NULL) {
        return FALSE;
    }
    if (!g_key_file_load_from_bytes (ga->keyfile, map_bytes, G_KEY_FILE_NONE, &error))
    {
        if (!g_error_matches (*error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
            g_warning ("Error loading key file: %s", *error->message);
        return FALSE;
    }
#endif
}
