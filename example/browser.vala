// valac browser.vala --pkg gtk+-3.0 --pkg giconfont-1.0

using Gtk;
using GIcon;

int main (string[] args) {
    Gtk.init (ref args);

    var window = new Window ();
    window.title = "Show Font Awesome";
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
    font.set_size (Gtk.IconSize.MENU);

    GLib.File f = null;
    GLib.Bytes bytes = null;
    if (args.length == 2){
        f = File.new_for_commandline_arg (args[1]);
    } else {
        f = File.new_for_commandline_arg ("https://github.com/yetist/giconfont/raw/master/giconfont/resource/fontawesome-webfont.map");
    }
    try {
        bytes = f.load_bytes (null, null);
    } catch (GLib.Error e) {
        print (e.message);
    }
    KeyFile file = new KeyFile ();
    try {
        file.load_from_bytes (bytes, KeyFileFlags.NONE);
        foreach (string group in file.get_groups ()) {
            foreach (string key in file.get_keys (group)) {
                var button = new Button.with_label(key);
                var img = font.get_image (key);
                button.set_image (img);
                button.clicked.connect (() => { print (key + " pressed\n"); });
                flowbox.add (button);
            }
        }
    } catch (GLib.KeyFileError e) {
        print (e.message);
    }

    window.add (scrolled);
    scrolled.add (flowbox);
    window.show_all ();

    Gtk.main ();
    return 0;
}
