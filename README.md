# GAwesome

Font Awesome support for Gtk3 applications

# Example


Use builtin icon font:

```
    GAwesome  *ga;
    GtkWidget *image;

    ga = g_awesome_new();
    image = g_awesome_get_image (ga, "facebook");

    gtk_button_set_image (GTK_BUTTON(button), image);
```

Use custom icon font:

```
    /* Test font from uri or path */
    g_awesome_set_font (ga, "https://at.alicdn.com/t/font_115436_p8ay96nf93g.ttf");
    g_awesome_set_code (ga, "https://github.com/yetist/gawesome/raw/master/example/code.map");
```

Snapshot for [example/main.c](./example/main.c)

![](./example/snapshot.png)
