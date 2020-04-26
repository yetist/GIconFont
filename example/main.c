/* vi: set sw=4 ts=4 wrap ai: */
/*
 * main.c: This file is part of ____
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

#include <gtk/gtk.h>
#include "gawesome.h"

gboolean press_button (GtkWidget *widget, GdkEvent *event, const gchar* user_data)
{
    g_print ("%s button pressed.\n", user_data);
    return TRUE;
}

GtkWidget* create_button (GAwesome* ga, const gchar *icon)
{
    GtkWidget *image;
    GtkWidget *button;

    button = gtk_button_new_with_label(icon);
    image = g_awesome_get_image (ga, icon);
    gtk_button_set_image (GTK_BUTTON(button), image);
    g_signal_connect (button, "button-press-event", G_CALLBACK (press_button), (gpointer) icon);
    return button;
}

static void activate (GtkApplication* app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *button;
    GAwesome*  ga;
    GdkRGBA rgba;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Test icon font");
    g_signal_connect (window, "delete-event", G_CALLBACK (gtk_main_quit), NULL);

    box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);

    ga = g_awesome_new();
    gdk_rgba_parse (&rgba, "blue");
    g_awesome_set_rgba(ga, &rgba);
    g_awesome_set_size(ga, GTK_ICON_SIZE_DIALOG);

    /* Test builtin icon font */
    gchar* icons[]={
        "twitter",
        "apple",
        "facebook",
        NULL
    };

    int i = 0 ;
    while (icons[i] != NULL ) {
        button = create_button (ga, icons[i]);
        gtk_box_pack_start (GTK_BOX(box), button, FALSE, FALSE, 0);
        i++;
    }

    /* Test font from uri or path */
    g_awesome_set_font (ga, "https://at.alicdn.com/t/font_115436_p8ay96nf93g.ttf");
    g_awesome_set_code (ga, "https://github.com/yetist/gawesome/raw/master/example/code.map");
    gdk_rgba_parse (&rgba, "red");
    g_awesome_set_rgba(ga, &rgba);
    g_awesome_set_size (ga, GTK_ICON_SIZE_MENU);
    button = create_button (ga, "weibo");
    gtk_box_pack_start (GTK_BOX(box), button, FALSE, FALSE, 0);

    gtk_container_add (GTK_CONTAINER (window), box);
    gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gawesome.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
