#include "Mystar.h"
#include <gtkmm.h>
#include <libglademm.h>
#include <libnotifymm.h>
#include <boost/lexical_cast.hpp>
using namespace Gtk;

class gMystar:public Window
{
    public:
        //gMystar(int argc, char* argv[]);
        gMystar();
        ~gMystar();
        static void help();
        static Mystar *mystar;
    protected:
        bool on_key_press_event(GdkEventKey *);
        void on_quit_button_clicked();
        void on_connect_button_clicked();
        void on_disconnect_button_clicked();
        void on_tray_clicked();
        static void show_message(const char *message);
        void show_window();
        static void hide_window();
        //static void *authen(void *);
        static void *test(void *);
        static void *change_ui(void *);

        Glib::RefPtr<Gnome::Glade::Xml> refXml;
        static Window *MainWindow;
        Entry *username_entry;
        Entry *password_entry;
        Entry *fakeAddress_entry;
        Entry *nickname_entry;
        static CheckButton *autologin_checkbutton;
        static Button *connect_button;
        Button *disconnect_button;
        Button *quit_button;
        static Label *status_label;
        static Glib::RefPtr<Gtk::StatusIcon> status_icon;

        pthread_t authen_thread;
        pthread_t change_ui_thread;

        static bool flag;

        static int window_x;
        static int window_y;
        static sigc::connection c;
        static Notify::Notification *n;
};
