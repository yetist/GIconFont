/* vi: set sw=4 ts=4 wrap ai expandtab: */
/*
 * g-awesome.c: This file is part of ____
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

#include <cairo.h>
#include <cairo-ft.h>
#include <ft2build.h>
#include "gawesome.h"
#include "gawesome-priv.h"
#include "gawesome-resource.h"
#include "debug.h"

enum {
    PROP_0,
    NUM_PROPERTIES
};

static GParamSpec *widget_props[NUM_PROPERTIES] = { NULL, };

struct _GAwesome
{
    GObject            object;
    FT_Library         library;
    FT_Face            ft_face;
    GBytes            *bytes;
    GKeyFile          *keyfile;
    cairo_font_face_t *font_face;
    GHashTable        *hash_table;

	GdkRGBA            default_rgba;
	GtkIconSize        default_size;
};

typedef struct _GAwesomeNameIcon GAwesomeNameIcon;
static GtkWidget* g_awesome_get_image_from_code (GAwesome *ga, GAwesomeCode code, GdkRGBA *rgba, GtkIconSize size);

G_DEFINE_TYPE (GAwesome, g_awesome, G_TYPE_OBJECT);

static void g_awesome_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec);
static void g_awesome_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec);

static void g_awesome_finalize (GObject *object)
{
    debug_print ("hi");
    GAwesome *ga;

    ga = G_AWESOME (object);

    if (ga->font_face != NULL)
        cairo_font_face_destroy (ga->font_face);
    if (ga->bytes != NULL)
        g_bytes_unref (ga->bytes);
    if (ga->ft_face != NULL)
        FT_Done_Face (ga->ft_face);
    if (ga->library != NULL)
        FT_Done_FreeType(ga->library);
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
}

gboolean freetype_face_new (GAwesome *ga, GError **error)
{
    debug_print ("hi");
    FT_Error status;

    GResource *resource;
    gsize size;
    gconstpointer buffer;

    status = FT_Init_FreeType (&ga->library);
    if (status != 0) {
        g_set_error (error,
                     g_quark_from_string ("gawesome"),
                     1,
                     "Error %d opening library.\n",
                     status);
        return FALSE;
    }

    /* load ttf font */
    resource = gawesome_get_resource();
    ga->bytes = g_resource_lookup_data (resource,
                                    "/cc/zhcn/gawesome/font.ttf",
                                     G_RESOURCE_LOOKUP_FLAGS_NONE,
                                     error);
    if (*error != NULL) {
        return FALSE;
    }

    buffer = g_bytes_get_data (ga->bytes, &size);
    status = FT_New_Memory_Face(ga->library,
                                buffer,
                                size,
                                0,
                                &ga->ft_face);
    if (status != 0) {
        g_set_error (error,
                     g_quark_from_string ("gawesome"),
                     2,
                     "Error %d opening built-in fonts %s.\n",
                     status,
                     "/cc/zhcn/gawesome/font.ttf");
        return FALSE;
    }
    return TRUE;
}

static void g_awesome_init (GAwesome *ga)
{
    debug_print ("hi");
    gboolean result = FALSE;
    GError *error = NULL;
    gint i;

    ga->keyfile = g_key_file_new ();
    result = freetype_face_new(ga, &error);
    if (!result) {
        g_print("%s\n", error->message);
    }
    ga->font_face = cairo_ft_font_face_create_for_ft_face (ga->ft_face, 0);
    ga->hash_table = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);

    for (i = 0; i < sizeof(gaNameIconArray)/sizeof(GAwesomeNameIcon); i++) {
        g_hash_table_insert (ga->hash_table, strdup(gaNameIconArray[i].name),
                             ((gpointer) (gulong) (gaNameIconArray[i].code)));
    }

    ga->default_rgba.red = 1.0;
    ga->default_rgba.green = 1.0;
    ga->default_rgba.blue = 1.0;
    ga->default_rgba.alpha = 1.0;
    ga->default_size = GTK_ICON_SIZE_BUTTON;
}

GAwesome* g_awesome_new (void)
{
    return g_object_new (G_TYPE_AWESOME, NULL);
}

static void g_awesome_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
    GAwesome *awesome;

    awesome = G_AWESOME (object);

    switch (prop_id)
    {

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

static GdkPixbuf* g_awesome_get_pixbuf_from_code (GAwesome *ga, GAwesomeCode code, GdkRGBA *rgba, gint size)
{
    cairo_t *cr;
    cairo_surface_t *surface;
    cairo_text_extents_t extents;
    gchar text[7] = {0};
    GdkPixbuf *pixbuf;
    double x,y;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, size, size);
    cr = cairo_create (surface);

    cairo_set_font_face (cr, ga->font_face);
    cairo_set_font_size (cr, size);
    cairo_set_source_rgba (cr, rgba->red, rgba->green, rgba->blue, rgba->alpha);

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

#if 0
static GtkWidget* g_awesome_get_image_from_code (GAwesome *ga, GAwesomeCode code, GdkRGBA *rgba, GtkIconSize icon_size)
{
    cairo_t *cr;
    cairo_surface_t *surface;
    cairo_text_extents_t extents;
    gint size;
    gchar text[7] = {0};
    GtkWidget *image;
    double x,y;

    size = gint_from_icon_size (icon_size);
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, size, size);
    cr = cairo_create (surface);

    cairo_set_font_face (cr, ga->font_face);
    cairo_set_font_size (cr, size);
    cairo_set_source_rgba (cr, rgba->red, rgba->green, rgba->blue, rgba->alpha);

    g_unichar_to_utf8 (code, text);
    cairo_text_extents (cr, text, &extents);
    x = (size - extents.width)/2 + extents.x_bearing;
    y = (size - extents.height)/2 - extents.y_bearing;
    cairo_move_to (cr, x, y);

    cairo_show_text (cr, text);

    image = gtk_image_new_from_surface (surface);
    cairo_surface_destroy (surface);
    cairo_destroy (cr);

    return image;
}
#endif

void g_awesome_set_default_rgba (GAwesome *ga, GdkRGBA *rgba)
{
    ga->default_rgba.red = rgba->red;
    ga->default_rgba.green = rgba->green;
    ga->default_rgba.blue = rgba->blue;
    ga->default_rgba.alpha = rgba->alpha;
}

void g_awesome_set_default_size (GAwesome *ga, GtkIconSize size)
{
    ga->default_size = size;
}

GdkPixbuf* g_awesome_get_pixbuf (GAwesome *ga, const gchar* name)
{
    return g_awesome_get_pixbuf_at_size (ga, name, ga->default_size);
}

GdkPixbuf* g_awesome_get_pixbuf_at_size (GAwesome *ga, const gchar* name, GtkIconSize size)
{
    return g_awesome_get_pixbuf_at_size_rgba (ga, name, size,  &ga->default_rgba);
}

GdkPixbuf* g_awesome_get_pixbuf_at_size_rgba (GAwesome *ga, const gchar* name, GtkIconSize icon_size, GdkRGBA *rgba)
{
    GdkPixbuf* pixbuf;
    gpointer code;
    gint size;

    code = g_hash_table_lookup (ga->hash_table, name);
    size = gint_from_icon_size (icon_size);

    pixbuf = g_awesome_get_pixbuf_from_code (ga, GPOINTER_TO_UINT (code), rgba, size);
    return pixbuf;
}

GtkWidget* g_awesome_get_image (GAwesome *ga, const gchar* name)
{
    return g_awesome_get_image_at_size_rgba (ga, name, ga->default_size, &ga->default_rgba);
}

GtkWidget* g_awesome_get_image_at_size (GAwesome *ga, const gchar* name, GtkIconSize size)
{
    return g_awesome_get_image_at_size_rgba (ga, name, size, &ga->default_rgba);
}

GtkWidget* g_awesome_get_image_at_size_rgba (GAwesome *ga, const gchar* name, GtkIconSize size, GdkRGBA *rgba)
{
    GdkPixbuf* pixbuf;
    GtkWidget *image;

    pixbuf = g_awesome_get_pixbuf_at_size_rgba (ga, name, size, rgba);
    image = gtk_image_new_from_pixbuf (pixbuf);
    g_object_unref (pixbuf);

    return image;
}
