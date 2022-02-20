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

#ifndef __GICON_FONT_H__
#define __GICON_FONT_H__  1

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GICON_TYPE_FONT (gicon_font_get_type ())
G_DECLARE_FINAL_TYPE (GIconFont, gicon_font, GICON, FONT, GObject)

GIconFont* gicon_font_new                       (void);
GIconFont* gicon_font_new_with_args             (const gchar* path, const gchar *map);
void       gicon_font_set_path                  (GIconFont *font, const gchar* path);
void       gicon_font_set_map                   (GIconFont *font, const gchar* map);
void       gicon_font_set_rgba                  (GIconFont *font, GdkRGBA *rgba);
void       gicon_font_set_size                  (GIconFont *font, GtkIconSize size);

GdkPixbuf* gicon_font_get_pixbuf                (GIconFont *font, const gchar* name);
GdkPixbuf* gicon_font_get_pixbuf_with_size      (GIconFont *font, const gchar* name,
                                                 GtkIconSize size);
GdkPixbuf* gicon_font_get_pixbuf_with_size_rgba (GIconFont *font, const gchar* name,
                                                 GtkIconSize size, GdkRGBA *rgba);

GtkWidget* gicon_font_get_image                 (GIconFont *font, const gchar* name);
GtkWidget* gicon_font_get_image_with_size       (GIconFont *font, const gchar* name,
                                                 GtkIconSize size);
GtkWidget* gicon_font_get_image_with_size_rgba  (GIconFont *font, const gchar* name,
                                                 GtkIconSize size,
                                                 GdkRGBA *rgba);

G_END_DECLS

#endif /* __GICON_FONT_H__ */
