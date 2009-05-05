#include <gtkmm.h>
#include <libglademm.h>
#include <libnotifymm.h>
#include <boost/lexical_cast.hpp>

#include "myqq.h"



using namespace Gtk;

class gMyqq
{
    public:
        gMyqq();
        ~gMyqq();
    protected:
        void on_tray_clicked();
        void show_window();
        void hide_window();
        void on_textview_enter_key_pressed();
        TextView *message_textview;
        Glib::RefPtr<Gnome::Glade::Xml> refXml;
		Glib::RefPtr<Gdk::Pixbuf>	ui_logo;
        Window *main_window;
        Entry *username_entry;
        Entry *password_entry;
        Entry *fakeAddress_entry;
        Entry *nickname_entry;
        static CheckButton *autologin_checkbutton;
        static Button *connect_button;
        Button *disconnect_button;
        Button *quit_button;
        Label *status_label;
        //Glib::RefPtr<Gtk::StatusIcon> status_icon;
        //Glib::RefPtr<Gtk::StatusBar> status_bar;

        pthread_t authen_thread;
        pthread_t change_ui_thread;

        int window_x;
        int window_y;
        static sigc::connection c;
        static Notify::Notification *n;
};
