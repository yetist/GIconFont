/*
 * Copyright (C) 2019-2022 Wu Xiaotian <yetist@gmail.com>
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

#include <cairo.h>
#include <cairo-ft.h>
#include <ft2build.h>
#include "config.h"
#include "gicon-font.h"

#if BUILT_IN_FONTS
#include "gicon-font-resource.h"
#endif

enum {
    PROP_0,
    PROP_RGBA,
    PROP_SIZE,
    PROP_FONT_PATH,
    PROP_MAP_PATH,
    NUM_PROPERTIES
};

static GParamSpec *widget_props[NUM_PROPERTIES] = { NULL, };

struct _GIconFont
{
    GObject            object;
    GdkRGBA           *rgba;
    GtkIconSize        size;

    FT_Library         library;
    FT_Face            ft_face;
    GBytes            *bytes;
    GHashTable        *hash_table;
    cairo_font_face_t *font_face;
    gchar             *font;
    gchar             *map;

};

G_DEFINE_TYPE (GIconFont, gicon_font, G_TYPE_OBJECT);

static void gicon_font_set_property (GObject *object, guint prop_id,
                                     const GValue *value, GParamSpec *pspec);
static void gicon_font_get_property (GObject *object, guint prop_id,
                                     GValue *value, GParamSpec *pspec);

static void gicon_font_finalize (GObject *object)
{
    GIconFont *font;

    font = GICON_FONT (object);

    if (font->font_face != NULL)
        cairo_font_face_destroy (font->font_face);
    if (font->ft_face != NULL)
        FT_Done_Face (font->ft_face);
    if (font->library != NULL)
        FT_Done_FreeType (font->library);
    if (font->bytes!= NULL)
        g_bytes_unref (font->bytes);
    if (font->hash_table != NULL)
        g_hash_table_destroy (font->hash_table);

    G_OBJECT_CLASS (gicon_font_parent_class)->finalize (object);
}

static void gicon_font_class_init (GIconFontClass *class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (class);

    gobject_class->finalize = gicon_font_finalize;
    gobject_class->set_property = gicon_font_set_property;
    gobject_class->get_property = gicon_font_get_property;

    widget_props[PROP_RGBA] = g_param_spec_boxed ("rgba",
                                                  "Default icon font RGBA",
                                                  "Default icon font color as a GdkRGBA",
                                                  GDK_TYPE_RGBA,
                                                  G_PARAM_READWRITE);
    widget_props[PROP_SIZE] = g_param_spec_enum ("size",
                                                 "Default icon font size",
                                                 "The GtkIconSize value that specifies the size of the rendered icon font",
                                                 GTK_TYPE_ICON_SIZE,
                                                 GTK_ICON_SIZE_MENU,
                                                 G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY);
    widget_props[PROP_FONT_PATH] = g_param_spec_string ("font-path",
                                                        "New font path",
                                                        "New font path",
                                                        NULL,
                                                        G_PARAM_READWRITE);
    widget_props[PROP_MAP_PATH] = g_param_spec_string ("map-path",
                                                       "New font map path",
                                                       "New font map path",
                                                       NULL,
                                                       G_PARAM_READWRITE);

    g_object_class_install_properties (gobject_class, NUM_PROPERTIES, widget_props);
}

static void gicon_font_init (GIconFont *font)
{
    font->size = GTK_ICON_SIZE_BUTTON;
    font->font = NULL;
    font->map = NULL;
    gdk_rgba_parse (font->rgba, "rgba(0,0,0,0)");
    font->hash_table = g_hash_table_new_full (g_str_hash,
                                              g_str_equal,
                                              g_free,
                                              NULL);
#if BUILT_IN_FONTS
    gicon_font_set_path (font, "resource://giconfont/font.ttf");
    gicon_font_set_map (font, "resource://giconfont/font.map");
#endif
}

/**
 * gicon_font_new:
 *
 *
 *
 * Return value: #GIconFont
 **/
GIconFont* gicon_font_new (void)
{
    return g_object_new (GICON_TYPE_FONT, NULL);
}

/**
 * gicon_font_new_with_args:
 * @path: the ttf font file path.
 * @map:  the font map file path.
 *
 * Creates a GIconFont with the given arguments. The value of args can be either a URI, an absolute path or a relative path resolved relative to the current working directory.
 *
 * Return value: GIconFont
 **/
GIconFont* gicon_font_new_with_args (const gchar* path, const gchar *map)
{
    return g_object_new (GICON_TYPE_FONT,
                         "font-path",
                         path,
                         "map-path",
                         map,
                         NULL);
}

/**
gicon_font_set_path:
@font: 
@path: 

*/
void gicon_font_set_path (GIconFont *font, const gchar* path)
{
    FT_Error status;
    gsize size;
    g_autoptr(GFile) file = NULL;
    g_autoptr(GError) error = NULL;
    gconstpointer buffer;

    /* init library */
    if (font->library == NULL) {
        status = FT_Init_FreeType (&font->library);
        if (status != 0) {
            g_warning ("Error %d opening library.\n", status);
            return;
        }
    }

    /* load ttf font */
    file = g_file_new_for_commandline_arg (path);
    if (font->bytes != NULL) {
        g_bytes_unref (font->bytes);
        font->bytes = NULL;
    }

    font->bytes = g_file_load_bytes (file, NULL, NULL, &error);
    if (font->bytes == NULL) {
        g_warning ("Error load bytes from %s: %s\n", path, error->message);
        return;
    }

    buffer = g_bytes_get_data (font->bytes, &size);
    if (buffer != NULL) {
        status = FT_New_Memory_Face (font->library,
                                     buffer,
                                     size,
                                     0,
                                     &font->ft_face);
        if (status != 0) {
            g_warning ("Error %d opening fonts %s.\n", status, path);
            return;
        }
        if (font->font_face != NULL)
            cairo_font_face_destroy (font->font_face);
        font->font_face = cairo_ft_font_face_create_for_ft_face (font->ft_face, 0);
    }
}

/**
gicon_font_set_map:
@font: 
@map: 

*/
void gicon_font_set_map (GIconFont *font, const gchar* map)
{
    g_autoptr(GFile) file = NULL;
    g_autoptr(GError) error = NULL;
    g_autoptr(GBytes) bytes = NULL;
    g_autoptr(GKeyFile) keyfile = NULL;
    gint i;
    gsize length;

    if (font->ft_face == NULL) {
        g_warning ("Please set font before set font map.\n");
        return;
    }

    /* load font map */
    file = g_file_new_for_commandline_arg (map);
    bytes = g_file_load_bytes (file, NULL, NULL, &error);
    if (bytes == NULL) {
        g_warning ("Error load bytes from %s: %s\n", map, error->message);
        return;
    }

    keyfile = g_key_file_new ();
    if (!g_key_file_load_from_bytes (keyfile, bytes, G_KEY_FILE_NONE, &error)) {
        if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT)) {
            g_warning ("Error loading key file: %s", error->message);
        }
        return;
    }

    g_auto(GStrv) keys = g_key_file_get_keys (keyfile,
                                              font->ft_face->family_name,
                                              &length,
                                              &error);
    if (keys == NULL) {
        g_warning ("Error get keys: %s", error->message);
        return;
    }

    if (font->hash_table != NULL)
        g_hash_table_destroy (font->hash_table);
    font->hash_table = g_hash_table_new_full (g_str_hash,
                                              g_str_equal,
                                              g_free,
                                              NULL);

    for (i = 0; i < length; i++) {
        gchar *val;
        val = g_key_file_get_value(keyfile,
                                   font->ft_face->family_name,
                                   keys[i],
                                   NULL);
        if (val != NULL) {
            guint32 code;
            code = g_ascii_strtoull (val, NULL, 16);
            if (code != 0) {
                g_hash_table_insert (font->hash_table,
                                     g_strdup(keys[i]),
                                     GUINT_TO_POINTER (code));
            }
        }
    }
}

/**
gicon_font_set_size:
@font: 
@size: 

*/
void gicon_font_set_size (GIconFont *font, GtkIconSize size)
{
    if (font->size == size)
        return;
    font->size = size;
    g_object_notify (G_OBJECT (font), "size");
}

/**
gicon_font_set_rgba:
@font: 
@rgba: 

*/
void gicon_font_set_rgba (GIconFont *font, GdkRGBA *rgba)
{
    if (font->rgba)
        gdk_rgba_free (font->rgba);
    if (rgba != NULL) {
        font->rgba = gdk_rgba_copy (rgba);
    } else {
        gdk_rgba_parse (font->rgba, "rgba(0,0,0,0)");
    }
    g_object_notify (G_OBJECT (font), "rgba");
}

static void gicon_font_set_property (GObject *object, guint prop_id,
                                     const GValue *value, GParamSpec *pspec)
{
    GIconFont *font;

    font = GICON_FONT (object);

    switch (prop_id)
        {
      case PROP_RGBA:
        gicon_font_set_rgba (font, g_value_get_boxed (value));
        break;
      case PROP_SIZE:
        gicon_font_set_size (font, g_value_get_enum (value));
        break;
      case PROP_FONT_PATH:
        gicon_font_set_path (font, g_value_get_string (value));
        break;
      case PROP_MAP_PATH:
        gicon_font_set_map (font, g_value_get_string (value));
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
        }
}

static void gicon_font_get_property (GObject *object, guint prop_id,
                                     GValue *value, GParamSpec *pspec)
{
    GIconFont *font;

    font = GICON_FONT (object);

    switch (prop_id)
        {
      case PROP_RGBA:
        g_value_set_boxed (value, font->rgba);
        break;
      case PROP_SIZE:
        g_value_set_enum (value, font->size);
        break;
      case PROP_FONT_PATH:
        g_value_set_string (value, font->font);
        break;
      case PROP_MAP_PATH:
        g_value_set_string (value, font->map);
        break;
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
        }
}

static gint gint_from_icon_size (GtkIconSize icon_size)
{
    gint size = 24;
    switch (icon_size) {
      case GTK_ICON_SIZE_INVALID:
      case GTK_ICON_SIZE_MENU:
      case GTK_ICON_SIZE_SMALL_TOOLBAR:
      case GTK_ICON_SIZE_BUTTON:
        size = 16;
        break;
      case GTK_ICON_SIZE_LARGE_TOOLBAR:
        size = 24;
        break;
      case GTK_ICON_SIZE_DND:
        size = 32;
        break;
      case GTK_ICON_SIZE_DIALOG:
        size = 48;
        break;
      default:
        size = 24;
        break;
    }
    return size;
}

static GdkPixbuf* gicon_font_get_pixbuf_from_code (GIconFont *font,
                                                   gunichar code,
                                                   gint size,
                                                   GdkRGBA *rgba)
{
    cairo_t *cr;
    cairo_surface_t *surface;
    cairo_text_extents_t extents;
    gchar text[7] = {0};
    GdkPixbuf *pixbuf;
    double x,y;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, size, size);
    cr = cairo_create (surface);
    if (font->font_face == NULL) {
        return NULL;
    }

    cairo_set_font_face (cr, font->font_face);
    cairo_set_font_size (cr, size);
    if (rgba != NULL) {
        cairo_set_source_rgba (cr, rgba->red, rgba->green, rgba->blue, rgba->alpha);
    }

    g_unichar_to_utf8 (code, text);
    cairo_text_extents (cr, text, &extents);
    x = (size - extents.width)/2 + extents.x_bearing;
    y = (size - extents.height)/2 - extents.y_bearing;
    cairo_move_to (cr, x, y);

    cairo_show_text (cr, text);

    pixbuf = gdk_pixbuf_get_from_surface (surface, 0, 0, size, size);

    cairo_surface_destroy (surface);
    cairo_destroy (cr);

    return pixbuf;
}

/**
 * gicon_font_get_pixbuf:
 * @font: 
 * @name: 
 *
 *
 *
 * Returns: (nullable) (transfer full): the #GdkPixbuf, or %NULL
 **/
GdkPixbuf* gicon_font_get_pixbuf (GIconFont *font, const gchar* name)
{
    return gicon_font_get_pixbuf_with_size (font, name, font->size);
}

/**
 * gicon_font_get_pixbuf_with_size:
 * @font: 
 * @name: 
 * @size: 
 *
 *
 *
 * Returns: (nullable) (transfer full): the #GdkPixbuf, or %NULL
 **/
GdkPixbuf* gicon_font_get_pixbuf_with_size (GIconFont *font, const gchar* name,
                                            GtkIconSize size)
{
    return gicon_font_get_pixbuf_with_size_rgba (font, name, size, font->rgba);
}

/**
 * gicon_font_get_pixbuf_with_size_rgba:
 * @font: 
 * @name: 
 * @size: 
 * @rgba: 
 *
 *
 *
 * Returns: (nullable) (transfer full): the #GdkPixbuf, or %NULL
 **/
GdkPixbuf* gicon_font_get_pixbuf_with_size_rgba (GIconFont *font,
                                                 const gchar* name,
                                                 GtkIconSize size,
                                                 GdkRGBA *rgba)
{
    gint i;
    gpointer code;
    GdkPixbuf* pixbuf;

    i = gint_from_icon_size (size);
    code = g_hash_table_lookup (font->hash_table, name);
    pixbuf = gicon_font_get_pixbuf_from_code (font, GPOINTER_TO_UINT(code), i, rgba);

    return pixbuf;
}

/**
 * gicon_font_get_image:
 * @font: 
 * @name: 
 *
 *
 *
 * Returns: (transfer full): the #GtkImage widget
 **/
GtkWidget* gicon_font_get_image (GIconFont *font, const gchar* name)
{
    return gicon_font_get_image_with_size_rgba (font, name, font->size, font->rgba);
}

/**
 * gicon_font_get_image_with_size:
 * @font: 
 * @name: 
 * @size: 
 *
 *
 *
 * Returns: (transfer full): the #GtkImage widget
 **/
GtkWidget* gicon_font_get_image_with_size (GIconFont *font, const gchar* name,
                                           GtkIconSize size)
{
    return gicon_font_get_image_with_size_rgba (font, name, size, font->rgba);
}

/**
 * gicon_font_get_image_with_size_rgba:
 * @font: 
 * @name: 
 * @size: 
 * @rgba: 
 *
 *
 * Returns: (transfer full): the #GtkImage widget
 **/
GtkWidget* gicon_font_get_image_with_size_rgba (GIconFont *font, const gchar* name,
                                                GtkIconSize size, GdkRGBA *rgba)
{
    GdkPixbuf* pixbuf;
    GtkWidget *image;

    pixbuf = gicon_font_get_pixbuf_with_size_rgba (font, name, size, rgba);
    image = gtk_image_new_from_pixbuf (pixbuf);
    if (pixbuf)
        g_object_unref (pixbuf);

    return image;
}
