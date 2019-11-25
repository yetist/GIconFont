/* vi: set sw=4 ts=4 wrap ai: */
/*
 * g-awesome.h: This file is part of ____
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

#ifndef __G_AWESOME_H__
#define __G_AWESOME_H__  1

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define G_TYPE_AWESOME              (g_awesome_get_type ())
G_DECLARE_FINAL_TYPE (GAwesome, g_awesome, G, AWESOME, GObject)

GAwesome*  g_awesome_new                     (void);

void       g_awesome_set_default_rgba        (GAwesome *ga, GdkRGBA *rgba);
void       g_awesome_set_default_size        (GAwesome *ga, GtkIconSize size);

GdkPixbuf* g_awesome_get_pixbuf              (GAwesome *ga, const gchar* name);
GdkPixbuf* g_awesome_get_pixbuf_at_size      (GAwesome *ga, const gchar* name, GtkIconSize size);
GdkPixbuf* g_awesome_get_pixbuf_at_size_rgba (GAwesome *ga, const gchar* name, GtkIconSize size, GdkRGBA *rgba);

GtkWidget* g_awesome_get_image               (GAwesome *ga, const gchar* name);
GtkWidget* g_awesome_get_image_at_size       (GAwesome *ga, const gchar* name, GtkIconSize size);
GtkWidget* g_awesome_get_image_at_size_rgba  (GAwesome *ga, const gchar* name, GtkIconSize size, GdkRGBA *rgba);

G_END_DECLS

#endif /* __G_AWESOME_H__ */
