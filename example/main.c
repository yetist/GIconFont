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

void hello (void)
{
	g_print ("Hello World\n");
}

void destroy (void)
{
	gtk_main_quit ();
}

int main (int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *image;
	GtkWidget *button;
	GAwesome*  ga;
	GdkRGBA rgba;

	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    button = gtk_button_new_with_label("确定");

    rgba.red = 0.3;
    rgba.green = 0.4;
    rgba.blue = 0.5;
    rgba.alpha = 1.0;
    ga = g_awesome_new();
	//image = g_awesome_get_image_from_code (ga, G_AWESOME_CODE_fa_500px, &rgba, GTK_ICON_SIZE_DIALOG);
	//image = g_awesome_get_image_from_code (ga, G_AWESOME_CODE_apple, &rgba, GTK_ICON_SIZE_DIALOG);
	//image = g_awesome_get_image_at_size_rgba (ga, "apple", GTK_ICON_SIZE_DIALOG, &rgba);
	image = g_awesome_get_image (ga, "apple");
    gtk_button_set_image (GTK_BUTTON(button), image);
	gtk_container_add (GTK_CONTAINER (window), button);
	gtk_widget_show_all (window);

	gtk_main ();

	return 0;
}
