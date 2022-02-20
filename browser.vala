// valac browser.vala --pkg gtk+-3.0 --pkg giconfont-1.0

using Gtk;
using GIcon;

int main (string[] args) {
    Gtk.init (ref args);

    var window = new Window ();
    window.title = "Browser icon fonts";
    window.border_width = 10;
    window.window_position = WindowPosition.CENTER;
    window.set_default_size (800, 600);
    window.destroy.connect (Gtk.main_quit);

    var scrolled = new ScrolledWindow (null, null);
    scrolled.set_policy (Gtk.PolicyType.AUTOMATIC, Gtk.PolicyType.AUTOMATIC);

    var flowbox = new FlowBox ();
    flowbox.set_valign (Gtk.Align.START);
    flowbox.set_max_children_per_line (30);
    flowbox.set_selection_mode (Gtk.SelectionMode.NONE);

    var font = new Font();
    GLib.Resource res = GIcon.Font.get_resource ();
    font.set_size (Gtk.IconSize.MENU);

    KeyFile file = new KeyFile ();
    try {
        try {
            file.load_from_file (args[1], KeyFileFlags.NONE);
        } catch (GLib.FileError f) {
        }
        foreach (string group in file.get_groups ()) {
            foreach (string key in file.get_keys (group)) {
                print ("Key: %s.%s = %s\n", group, key, file.get_value (group, key));

                var button = new Button.with_label(key);
                var img = font.get_image (key);
                button.set_image (img);
                button.clicked.connect (() => {
                        print (key + " pressed\n");
                        });
                flowbox.add (button);
            }
        }
    } catch (GLib.KeyFileError e) {
    }

    window.add (scrolled);
    scrolled.add (flowbox);
    window.show_all ();

    Gtk.main ();
    return 0;
}


/*
using Gtk;

public class Example : Window
{
    public Example()
    {
        this.title = "FlowBox";
        this.destroy.connect(Gtk.main_quit);

        var flowbox = new FlowBox();
        this.add(flowbox);

        var button = new Button.with_label("Button in FlowBox");
        flowbox.insert(button, 0);

        var togglebutton = new ToggleButton.with_label("ToggleButton in FlowBox");
        flowbox.insert(togglebutton, 1);

        var checkbutton = new CheckButton.with_label("CheckButton in FlowBox");
        flowbox.insert(checkbutton, -1);
    }

    public static int main(string[] args)
    {
        Gtk.init(ref args);

        var window = new Example();
        window.show_all();

        Gtk.main();

        return 0;
    }
}

*/
