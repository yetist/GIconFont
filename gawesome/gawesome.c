/* vi: set sw=4 ts=4 wrap ai expandtab: */
/*
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

#include <cairo.h>
#include <cairo-ft.h>
#include <ft2build.h>
#include "config.h"
#include "gawesome.h"

#if BUILT_IN_FONTS
#include "gawesome-resource.h"
#endif

enum {
    PROP_0,
    PROP_ICON_RGBA,
    PROP_ICON_SIZE,
    PROP_FONT_PATH,
    PROP_MAP_PATH,
    NUM_PROPERTIES
};

static GParamSpec *widget_props[NUM_PROPERTIES] = { NULL, };

struct _GAwesome
{
    GObject            object;
    GdkRGBA           *icon_rgba;
    GtkIconSize        icon_size;

    FT_Library         library;
    FT_Face            ft_face;
    GBytes            *bytes;
    GHashTable        *hash_table;
    cairo_font_face_t *font_face;
    gchar             *font;
    gchar             *map;

};

typedef struct _GAwesomeNameIcon GAwesomeNameIcon;

G_DEFINE_TYPE (GAwesome, g_awesome, G_TYPE_OBJECT);

static void g_awesome_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void g_awesome_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

static void g_awesome_finalize (GObject *object)
{
    GAwesome *ga;

    ga = G_AWESOME (object);

    if (ga->font_face != NULL)
        cairo_font_face_destroy (ga->font_face);
    if (ga->ft_face != NULL)
        FT_Done_Face (ga->ft_face);
    if (ga->library != NULL)
        FT_Done_FreeType(ga->library);
    if (ga->bytes!= NULL)
        g_bytes_unref(ga->bytes);
    if (ga->hash_table != NULL)
        g_hash_table_destroy(ga->hash_table);

	G_OBJECT_CLASS (g_awesome_parent_class)->finalize (object);
}

static void g_awesome_class_init (GAwesomeClass *class)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (class);

    gobject_class->finalize = g_awesome_finalize;
    gobject_class->set_property = g_awesome_set_property;
    gobject_class->get_property = g_awesome_get_property;

    widget_props[PROP_ICON_RGBA] = g_param_spec_boxed ("icon-rgba",
                                                       "Default icon RGBA",
                                                       "Default icon color as a GdkRGBA",
                                                       GDK_TYPE_RGBA,
                                                       G_PARAM_READWRITE);
    widget_props[PROP_ICON_SIZE] = g_param_spec_enum ("icon-size",
                                                      "Default icon size",
                                                      "The GtkIconSize value that specifies the size of the rendered icon",
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

static void g_awesome_init (GAwesome *ga)
{
    ga->icon_size = GTK_ICON_SIZE_BUTTON;
    ga->font = NULL;
    ga->map = NULL;
    gdk_rgba_parse (ga->icon_rgba, "rgba(0,0,0,0)");
    ga->hash_table = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
#if BUILT_IN_FONTS
    g_awesome_set_font (ga, "resource://cc/zhcn/gawesome/font.ttf");
    g_awesome_set_code (ga, "resource://cc/zhcn/gawesome/font.map");
#endif
}

GAwesome* g_awesome_new (void)
{
    return g_object_new (G_TYPE_AWESOME, NULL);
}

GAwesome* g_awesome_new_with_font (const gchar* font, const gchar *code)
{
    return g_object_new (G_TYPE_AWESOME, "font-path", font, "map-path", code, NULL);
}

void g_awesome_set_font (GAwesome *ga, const gchar* font)
{
    FT_Error status;
    gsize size;
    g_autoptr(GFile) file = NULL;
    g_autoptr(GError) error = NULL;
    gconstpointer buffer;

    /* init library */
    if (ga->library == NULL) {
        status = FT_Init_FreeType (&ga->library);
        if (status != 0) {
            g_warning ("Error %d opening library.\n", status);
            return;
        }
    }

    /* load ttf font */
    file = g_file_new_for_commandline_arg (font);
    if (ga->bytes != NULL) {
        g_bytes_unref(ga->bytes);
        ga->bytes = NULL;
    }

    ga->bytes = g_file_load_bytes (file, NULL, NULL, &error);
    if (ga->bytes == NULL) {
        g_warning ("Error load bytes from %s: %s\n", font, error->message);
        return;
    }

    buffer = g_bytes_get_data (ga->bytes, &size);
    if (buffer != NULL) {
        status = FT_New_Memory_Face(ga->library,
                                    buffer,
                                    size,
                                    0,
                                    &ga->ft_face);
        if (status != 0) {
            g_warning ("Error %d opening fonts %s.\n", status, font);
            return;
        }
        if (ga->font_face != NULL)
            cairo_font_face_destroy (ga->font_face);
        ga->font_face = cairo_ft_font_face_create_for_ft_face (ga->ft_face, 0);
    }
}

void g_awesome_set_code (GAwesome *ga, const gchar* code)
{
    g_autoptr(GFile) file = NULL;
    g_autoptr(GError) error = NULL;
    g_autoptr(GBytes) bytes = NULL;
    g_autoptr(GKeyFile) keyfile = NULL;
    gint i;
    gsize length;

    if (ga->ft_face == NULL) {
        g_warning ("Please set font before set code\n");
        return;
    }

    /* load code map */
    file = g_file_new_for_commandline_arg (code);
    bytes = g_file_load_bytes (file, NULL, NULL, &error);
    if (bytes == NULL) {
        g_warning ("Error load bytes from %s: %s\n", code, error->message);
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
                                              ga->ft_face->family_name,
                                              &length,
                                              &error);
    if (keys == NULL) {
        g_warning ("Error get keys: %s", error->message);
        return;
    }

    if (ga->hash_table != NULL)
        g_hash_table_destroy(ga->hash_table);
    ga->hash_table = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);

    for (i = 0; i < length; i++) {
        gchar *val;
        val = g_key_file_get_value(keyfile, ga->ft_face->family_name, keys[i], NULL);
        if (val != NULL) {
            guint32 code;
            code = g_ascii_strtoull (val, NULL, 16);
            if (code != 0) {
                g_hash_table_insert (ga->hash_table, g_strdup(keys[i]), GUINT_TO_POINTER(code));
            }
        }
    }
}

void g_awesome_set_size (GAwesome *ga, GtkIconSize icon_size)
{
    if (ga ->icon_size == icon_size)
        return;
    ga->icon_size = icon_size;
    g_object_notify (G_OBJECT (ga), "icon-size");
}

void g_awesome_set_rgba (GAwesome *ga, GdkRGBA *rgba)
{
    if (ga->icon_rgba)
        gdk_rgba_free (ga->icon_rgba);
    if (rgba != NULL) {
        ga->icon_rgba = gdk_rgba_copy (rgba);
    } else {
        gdk_rgba_parse (ga->icon_rgba, "rgba(0,0,0,0)");
    }
    g_object_notify (G_OBJECT (ga), "icon-rgba");
}

static void g_awesome_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    GAwesome *awesome;

    awesome = G_AWESOME (object);

    switch (prop_id)
    {
        case PROP_ICON_RGBA:
            g_awesome_set_rgba (awesome, g_value_get_boxed (value));
            break;
        case PROP_ICON_SIZE:
            g_awesome_set_size (awesome, g_value_get_enum (value));
            break;
        case PROP_FONT_PATH:
            g_awesome_set_font (awesome, g_value_get_string (value));
            break;
        case PROP_MAP_PATH:
            g_awesome_set_code (awesome, g_value_get_string (value));
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
            break;
    }
}

static void g_awesome_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
    GAwesome *awesome;

    awesome = G_AWESOME (object);

    switch (prop_id)
    {
        case PROP_ICON_RGBA:
            g_value_set_boxed (value, awesome->icon_rgba);
            break;
        case PROP_ICON_SIZE:
            g_value_set_enum (value, awesome->icon_size);
            break;
        case PROP_FONT_PATH:
            g_value_set_string (value, awesome->font);
            break;
        case PROP_MAP_PATH:
            g_value_set_string (value, awesome->map);
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

static GdkPixbuf* g_awesome_get_pixbuf_from_code (GAwesome *ga, gunichar code, gint size, GdkRGBA *rgba)
{
    cairo_t *cr;
    cairo_surface_t *surface;
    cairo_text_extents_t extents;
    gchar text[7] = {0};
    GdkPixbuf *pixbuf;
    double x,y;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, size, size);
    cr = cairo_create (surface);
    if (ga->font_face == NULL) {
        return NULL;
    }

    cairo_set_font_face (cr, ga->font_face);
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

GdkPixbuf* g_awesome_get_pixbuf (GAwesome *ga, const gchar* name)
{
    return g_awesome_get_pixbuf_at_size (ga, name, ga->icon_size);
}

GdkPixbuf* g_awesome_get_pixbuf_at_size (GAwesome *ga, const gchar* name, GtkIconSize size)
{
    return g_awesome_get_pixbuf_at_size_rgba (ga, name, size, ga->icon_rgba);
}

GdkPixbuf* g_awesome_get_pixbuf_at_size_rgba (GAwesome *ga, const gchar* name, GtkIconSize icon_size, GdkRGBA *rgba)
{
    gint size;
    gpointer code;
    GdkPixbuf* pixbuf;

    size = gint_from_icon_size (icon_size);
    code = g_hash_table_lookup (ga->hash_table, name);
    pixbuf = g_awesome_get_pixbuf_from_code (ga, GPOINTER_TO_UINT(code), size, rgba);

    return pixbuf;
}

GtkWidget* g_awesome_get_image (GAwesome *ga, const gchar* name)
{
    return g_awesome_get_image_at_size_rgba (ga, name, ga->icon_size, ga->icon_rgba);
}

GtkWidget* g_awesome_get_image_at_size (GAwesome *ga, const gchar* name, GtkIconSize size)
{
    return g_awesome_get_image_at_size_rgba (ga, name, size, ga->icon_rgba);
}

GtkWidget* g_awesome_get_image_at_size_rgba (GAwesome *ga, const gchar* name, GtkIconSize size, GdkRGBA *rgba)
{
    GdkPixbuf* pixbuf;
    GtkWidget *image;

    pixbuf = g_awesome_get_pixbuf_at_size_rgba (ga, name, size, rgba);
    image = gtk_image_new_from_pixbuf (pixbuf);
    if (pixbuf)
        g_object_unref (pixbuf);

    return image;
}
