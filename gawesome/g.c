struct _GAwesome
{
    GObject object;
    FT_Library library;
    FT_Face ft_face;
    GBytes *bytes;
    cairo_font_face_t *font_face;
    GHashTable *hash_table;
};

typedef struct _GAwesomeNameIcon GAwesomeNameIcon;
static GtkWidget* g_awesome_get_image_from_code (GAwesome *ga, GAwesomeCode code, GdkRGBA *rgba, GtkIconSize size);

static void g_awesome_init (GAwesome *self); static void g_awesome_class_init (GAwesomeClass *klass); static GType g_awesome_get_type_once (void); static gpointer g_awesome_parent_class = 
# 50 "../gawesome/gawesome.c" 3 4
((void *)0)
# 50 "../gawesome/gawesome.c"
; static gint GAwesome_private_offset; static void g_awesome_class_intern_init (gpointer klass) { g_awesome_parent_class = g_type_class_peek_parent (klass); if (GAwesome_private_offset != 0) g_type_class_adjust_private_offset (klass, &GAwesome_private_offset); g_awesome_class_init ((GAwesomeClass*) klass); } __attribute__((__unused__)) static inline gpointer g_awesome_get_instance_private (GAwesome *self) { return (((gpointer) ((guint8*) (self) + (glong) (GAwesome_private_offset)))); } GType g_awesome_get_type (void) { static volatile gsize g_define_type_id__volatile = 0; if ((__extension__ ({ _Static_assert (sizeof *(&g_define_type_id__volatile) == sizeof (gpointer), "Expression evaluates to false"); (void) (0 ? (gpointer) *(&g_define_type_id__volatile) : 0); (!(__extension__ ({ _Static_assert (sizeof *(&g_define_type_id__volatile) == sizeof (gpointer), "Expression evaluates to false"); guint64 gapg_temp = __atomic_load_8 ((&g_define_type_id__volatile), 5); (gpointer) gapg_temp; })) && g_once_init_enter (&g_define_type_id__volatile)); }))) { GType g_define_type_id = g_awesome_get_type_once (); (__extension__ ({ _Static_assert (sizeof *(&g_define_type_id__volatile) == sizeof (gpointer), "Expression evaluates to false"); (void) (0 ? *(&g_define_type_id__volatile) = (g_define_type_id) : 0); g_once_init_leave ((&g_define_type_id__volatile), (gsize) (g_define_type_id)); })); } return g_define_type_id__volatile; } __attribute__((noinline)) static GType g_awesome_get_type_once (void) { GType g_define_type_id = g_type_register_static_simple (((GType) ((20) << (2))), g_intern_static_string ("GAwesome"), sizeof (GAwesomeClass), (GClassInitFunc)(void (*)(void)) g_awesome_class_intern_init, sizeof (GAwesome), (GInstanceInitFunc)(void (*)(void)) g_awesome_init, (GTypeFlags) 0); { {{};} } return g_define_type_id; };

static void g_awesome_finalize (GObject *object)
{
    do { g_printerr ("../gawesome/gawesome.c" ":" "54" ":" "hi" "\n"); } while (0);
    GAwesome *ga;

    ga = G_AWESOME (object);

    if (ga->font_face != 
# 59 "../gawesome/gawesome.c" 3 4
                        ((void *)0)
# 59 "../gawesome/gawesome.c"
                            )
        cairo_font_face_destroy (ga->font_face);
    if (ga->bytes != 
# 61 "../gawesome/gawesome.c" 3 4
                    ((void *)0)
# 61 "../gawesome/gawesome.c"
                        )
        g_bytes_unref (ga->bytes);
    if (ga->ft_face != 
# 63 "../gawesome/gawesome.c" 3 4
                      ((void *)0)
# 63 "../gawesome/gawesome.c"
                          )
        FT_Done_Face (ga->ft_face);
    if (ga->library != 
# 65 "../gawesome/gawesome.c" 3 4
                      ((void *)0)
# 65 "../gawesome/gawesome.c"
                          )
        FT_Done_FreeType(ga->library);
    if (ga->hash_table != 
# 67 "../gawesome/gawesome.c" 3 4
                         ((void *)0)
# 67 "../gawesome/gawesome.c"
                             )
        g_hash_table_destroy(ga->hash_table);

    ((((GObjectClass*) g_type_check_class_cast ((GTypeClass*) ((g_awesome_parent_class)), (((GType) ((20) << (2))))))))->finalize (object);
}

static void g_awesome_class_init (GAwesomeClass *class)
{
    do { g_printerr ("../gawesome/gawesome.c" ":" "75" ":" "hi" "\n"); } while (0);
    GObjectClass *gobject_class = ((((GObjectClass*) g_type_check_class_cast ((GTypeClass*) ((class)), (((GType) ((20) << (2))))))));
    gobject_class->finalize = g_awesome_finalize;
}

gboolean freetype_face_new (GAwesome *ga, GError **error)
{
    do { g_printerr ("../gawesome/gawesome.c" ":" "82" ":" "hi" "\n"); } while (0);
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
        return (0);
    }

    resource = gawesome_get_resource();
    ga->bytes = g_resource_lookup_data (resource,
                                    "/cc/zhcn/gawesome/font.ttf",
                                     G_RESOURCE_LOOKUP_FLAGS_NONE,
                                     error);
    if (*error != 
# 107 "../gawesome/gawesome.c" 3 4
                 ((void *)0)
# 107 "../gawesome/gawesome.c"
                     ) {
        return (0);
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
        return (0);
    }
# 138 "../gawesome/gawesome.c"
    return (!(0));
}

static void g_awesome_init (GAwesome *ga)
{
    do { g_printerr ("../gawesome/gawesome.c" ":" "143" ":" "hi" "\n"); } while (0);
    gboolean result = (0);
    GError *error = 
# 145 "../gawesome/gawesome.c" 3 4
                   ((void *)0)
# 145 "../gawesome/gawesome.c"
                       ;
    gint i;

    if (ga->font_face == 
# 148 "../gawesome/gawesome.c" 3 4
                        ((void *)0)
# 148 "../gawesome/gawesome.c"
                            ) {
        g_print("ga->font_face is NULL\n");
    }

    result = freetype_face_new(ga, &error);
    if (!result) {
        g_print("%s\n", error->message);
    }
    ga->font_face = cairo_ft_font_face_create_for_ft_face (ga->ft_face, 0);
    ga->hash_table = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, 
# 157 "../gawesome/gawesome.c" 3 4
                                                                            ((void *)0)
# 157 "../gawesome/gawesome.c"
                                                                                );

    for (i = 0; i < sizeof(gaNameIconArray)/sizeof(GAwesomeNameIcon); i++) {
        g_hash_table_insert (ga->hash_table, strdup(gaNameIconArray[i].name),
                             ((gpointer) (gulong) (gaNameIconArray[i].code)));
    }
}

GAwesome* g_awesome_new (void)
{
    do { g_printerr ("../gawesome/gawesome.c" ":" "167" ":" "hi" "\n"); } while (0);
    return g_object_new ((g_awesome_get_type ()), 
# 168 "../gawesome/gawesome.c" 3 4
                                        ((void *)0)
# 168 "../gawesome/gawesome.c"
                                            );
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

GdkPixbuf* g_awesome_get_pixbuf (GAwesome *ga, const gchar* name);
GdkPixbuf* g_awesome_get_pixbuf_at_size (GAwesome *ga, const gchar* name, GtkIconSize size);
GdkPixbuf* g_awesome_get_pixbuf_at_size_rgba (GAwesome *ga, const gchar* name, GtkIconSize size, GdkRGBA *rgba);

GtkWidget* g_awesome_get_image (GAwesome *ga, const gchar* name);
GtkWidget* g_awesome_get_image_at_size (GAwesome *ga, const gchar* name, GtkIconSize size);

GtkWidget* g_awesome_get_image_at_size_rgba (GAwesome *ga, const gchar* name, GtkIconSize size, GdkRGBA *rgba)
{
    GAwesomeCode code;
    gpointer value;

    value = g_hash_table_lookup (ga->hash_table, name);
    code = ((guint) (gulong) (value));
    return g_awesome_get_image_from_code (ga, code, rgba, size);
}
