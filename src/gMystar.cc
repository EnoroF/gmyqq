#include "gMystar.h"
#include <pthread.h>
/* static member */
Mystar *gMystar::mystar;
bool gMystar::flag;
Glib::RefPtr<Gtk::StatusIcon> gMystar::status_icon;
Window *gMystar::MainWindow;
CheckButton *gMystar::autologin_checkbutton;
Button *gMystar::connect_button;
Label *gMystar::status_label;
int gMystar::window_x;
int gMystar::window_y;
sigc::connection gMystar::c;
Notify::Notification *gMystar::n;
/*
gMystar::gMystar(int argc, char *argv[])
{
    Main kit(argc, argv);
    mystar = new Mystar();
    flag = true;

    //tray
    status_icon = StatusIcon::create_from_file("/usr/gMystar/disconnect.png");
    if(status_icon)
    {
        status_icon->set_tooltip("用gtkmm和glade做的锐捷客户端.");
        status_icon->signal_activate().connect(sigc::mem_fun(*this, &gMystar::on_tray_clicked));
    }
    else
    {
        cout<<"create tray icon error!\n";
    }

    refXml = Gnome::Glade::Xml::create("/etc/gMystar/ui.glade");
    //refXml = Gnome::Glade::Xml::create("./gMystar.glade");
    MainWindow= NULL;
    MainWindow= refXml->get_widget("gMystar", MainWindow);

    nickname_entry= NULL;
    nickname_entry = refXml->get_widget("nickname", nickname_entry);
    nickname_entry->set_text(mystar->user.get_nickname());

    username_entry= NULL;
    username_entry= refXml->get_widget("username", username_entry);
    username_entry->set_text(mystar->user.get_username());

    password_entry= NULL;
    password_entry= refXml->get_widget("password", password_entry);
    password_entry->set_text(mystar->user.get_password());

    fakeAddress_entry = NULL;
    fakeAddress_entry = refXml->get_widget("fakeAddress", fakeAddress_entry);
    fakeAddress_entry->set_text(mystar->user.get_fakeAddress());

    autologin_checkbutton = NULL;
    autologin_checkbutton = refXml->get_widget("autologin_checkbutton",autologin_checkbutton);

    connect_button = NULL;
    connect_button = refXml->get_widget("connect_button", connect_button);
    if(connect_button)
    {
        gMystar::c = connect_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_connect_button_clicked));
    }


    disconnect_button = NULL;
    disconnect_button = refXml->get_widget("disconnect_button", disconnect_button);
    if(disconnect_button)
    {
        gMystar::c = disconnect_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_disconnect_button_clicked));
    }

    quit_button = NULL;
    quit_button = refXml->get_widget("quit_button", quit_button);
    if(quit_button)
    {
        quit_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_quit_button_clicked));
    }

    status_label = NULL;
    status_label = refXml->get_widget("status",status_label);

    MainWindow->set_focus(*connect_button);
    MainWindow->show();

    if(mystar->autologin)
        on_connect_button_clicked();

    kit.run();          //这样的话在hide时不退出程序
}
*/
gMystar::gMystar()
{
    mystar = new Mystar();
    flag = true;

    //tray
    status_icon = StatusIcon::create_from_file("/usr/share/gMystar/data/disconnect.png");
    if(status_icon)
    {
        status_icon->set_tooltip("用gtkmm和glade做的锐捷客户端.");
        status_icon->signal_activate().connect(sigc::mem_fun(*this, &gMystar::on_tray_clicked));
    }
    else
    {
        cout<<"create tray icon error!\n";
    }

    refXml = Gnome::Glade::Xml::create("/usr/share/gMystar/data/gMystar.glade","main_window");
	if(!refXml)
		exit(271);

	VBox* main_window = NULL;
    main_window = refXml->get_widget("main_window", main_window);

    nickname_entry= NULL;
    nickname_entry = refXml->get_widget("nickname", nickname_entry);
    nickname_entry->set_text(mystar->user.get_nickname());

    username_entry= NULL;
    username_entry= refXml->get_widget("username", username_entry);
    username_entry->set_text(mystar->user.get_username());

    password_entry= NULL;
    password_entry= refXml->get_widget("password", password_entry);
    password_entry->set_text(mystar->user.get_password());

    fakeAddress_entry = NULL;
    fakeAddress_entry = refXml->get_widget("fakeAddress", fakeAddress_entry);
    fakeAddress_entry->set_text(mystar->user.get_fakeAddress());

    autologin_checkbutton = NULL;
    autologin_checkbutton = refXml->get_widget("autologin_checkbutton",autologin_checkbutton);

    connect_button = NULL;
    connect_button = refXml->get_widget("connect_button", connect_button);
    if(connect_button)
    {
        gMystar::c = connect_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_connect_button_clicked));
    }


    disconnect_button = NULL;
    disconnect_button = refXml->get_widget("disconnect_button", disconnect_button);
    if(disconnect_button)
    {
        gMystar::c = disconnect_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_disconnect_button_clicked));
    }

    quit_button = NULL;
    quit_button = refXml->get_widget("quit_button", quit_button);
    if(quit_button)
    {
        quit_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_quit_button_clicked));
    }

    status_label = NULL;
    status_label = refXml->get_widget("status",status_label);

    /*
	VBox* main_window = dynamic_cast<VBox*>(refXml->get_widget("main_window"));

	nickname_entry = dynamic_cast<Entry*>(refXml->get_widget("nickname"));
    nickname_entry->set_text(mystar->user.get_nickname());

	username_entry = dynamic_cast<Entry*>(refXml->get_widget("name"));
    username_entry->set_text(mystar->user.get_username());

	password_entry = dynamic_cast<Entry*>(refXml->get_widget("password"));
    password_entry->set_text(mystar->user.get_password());

	fakeAddress_entry = dynamic_cast<Entry*>(refXml->get_widget("fakeAddress"));
    fakeAddress_entry->set_text(mystar->user.get_fakeAddress());

    autologin_checkbutton = dynamic_cast<CheckButton*>(refXml->get_widget("autologin_checkbutton"));

    connect_button = dynamic_cast<Button*>(refXml->get_widget("connect_button"));
    if(connect_button)
    {
        gMystar::c = connect_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_connect_button_clicked));
    }


    disconnect_button = dynamic_cast<Button*>(refXml->get_widget("disconnect_button"));
    if(disconnect_button)
    {
        gMystar::c = disconnect_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_disconnect_button_clicked));
    }

    quit_button = dynamic_cast<Button*>(refXml->get_widget("quit_button"));
    if(quit_button)
    {
        quit_button->signal_clicked().connect(sigc::mem_fun(*this, &gMystar::on_quit_button_clicked));
    }

    //status_label = dynamic_cast<Label*>(refXml->get_widget("status"));
    */

    add(*main_window);
	show_all();

    if(mystar->autologin)
        on_connect_button_clicked();
}
gMystar::~gMystar()
{
}
void gMystar::on_quit_button_clicked()
{
    //MainWindow->hide();//这样只是隐藏了，如何结束呢？
    hide();//这样只是隐藏了，如何结束呢？
}
void gMystar::on_disconnect_button_clicked()
{
    mystar->logout(SIGINT);
    status_label->set_label("logout...");
    if(!pthread_cancel(authen_thread))
        cout<<"取消认证。。。\n";
}
void gMystar::on_connect_button_clicked()
{
    cout<<"用于连接的账号信息是:"<<endl;
    char str[20];
    strcpy(str,nickname_entry->get_text().c_str());
    mystar->user.set_nickname(str);

    strcpy(str,username_entry->get_text().c_str());
    mystar->user.set_username(str);

    strcpy(str,password_entry->get_text().c_str());
    mystar->user.set_password(str);

    strcpy(str,fakeAddress_entry->get_text().c_str());
    mystar->user.set_fakeAddress(str);

    int res = pthread_create(&authen_thread, NULL,gMystar::test, NULL);
    if(res!=0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    int res2 = pthread_create(&change_ui_thread, NULL,gMystar::change_ui, NULL);
    if(res2!=0)
    {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
}
void gMystar::help()
{
    printf("to use no gui mode, please type --nogui\n");
}
void gMystar::show_message(const char *message)
{
    cout<<message;
    status_label->set_text(message);
}
void gMystar::on_tray_clicked()
{
    if(MainWindow->is_visible())
        hide_window();
    else
        show_window();
}
void gMystar::show_window()
{
    MainWindow->move(window_x, window_y);
    cout<<"resume the position("<<window_x<<","<<window_y<<")"<<endl;
    MainWindow->show();
}
void gMystar::hide_window()
{
    //MainWindow->get_position(window_x, window_y);
    //cout<<"save the current position ("<<window_x<<","<<window_y<<")"<<endl;
    //hide();
}
void *gMystar::test(void *a)
{
    mystar->authen();
}
void *gMystar::change_ui(void *a)
{
    while(flag)
    {
        int status = mystar->get_status();
        switch(status)
        {
            case 0:
                status_label->set_label(">> Searching for server...\n");
                break;
            case 1:
                status_label->set_label(" Sending user name...");
                break;
            case 2:
                status_label->set_label(" Sending password...");
                break;
            case 3:
                status_icon->set_from_file("/usr/share/gMystar/data/connect.png");
                status_label->set_label("keep sending echo...");

                Notify::init("Icon test");
                n = new Notify::Notification("gMystar", "连接成功", "appointment-new");
                n->set_timeout(1000); //1 seconds
                n->show();
                
                hide_window();
                if(autologin_checkbutton->get_active())
                    mystar->autologin=true;
                else
                    mystar->autologin=false;
                mystar->save_file();
                flag=false;
                break;
            case 5://网口断开状态
                n = new Notify::Notification("gMystar", "网络断开，请检查网络。。", "appointment-new");
                n->set_timeout(1000); //1 seconds
                n->show();
                flag=false;
            default:
                flag=true;
                break;
        }
    }
}
bool gMystar::on_key_press_event(GdkEventKey *ev)
{
    if(ev->type!=GDK_KEY_PRESS)
        return Window::on_key_press_event(ev);
    switch(ev->keyval)
    {
        case GDK_w:
            if(ev->state & GDK_CONTROL_MASK)
                //hide_window();
                hide();
        default:
            return Window::on_key_press_event(ev);
    }
    return true;
}
