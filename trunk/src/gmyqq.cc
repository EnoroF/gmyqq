#include <pthread.h>
#include "gmyqq.h"

/* static member */

gMyqq::gMyqq()
{
    //tray
    //status_icon = StatusIcon::create_from_file("../data/gmyqq.png");
    //if(status_icon)
    //{
     //   status_icon->set_tooltip("gmyqq, a qq client for linux.");
      //  status_icon->signal_activate().connect(sigc::mem_fun(*this, &gMyqq::on_tray_clicked));
    //}
    //else
   // {
    //    cout<<"create tray icon error!\n";
   // }

    refXml = Gnome::Glade::Xml::create("../data/gmyqq.glade","main_window");
	if(!refXml)
		exit(271);


    //status_bar=NULL;
    //status_bar = refXml->get_widget("statusbar",status_bar);
    //status_bar->set_text("welcome to gmyqq!");

    //status_label=NULL;
    //status_label= refXml->get_widget("message_label",status_bar);
    //status_label->set_text("welcome to gmyqq!");
    
    message_textview=NULL;
    message_textview = refXml->get_widget("message_textview",message_textview);
    //message_textview->signal_clicked().connect();

	main_window = NULL;
    main_window = refXml->get_widget("main_window", main_window);

    main_window->set_default_size(800,600);
    main_window->set_title("gmyqq");
    //ui_logo  = Gdk::Pixbuf::create_from_file("/home/laputa/Projects/gmyqq/data/gmchess.png");
	//main_window->set_icon(ui_logo);
    main_window->set_focus(*message_textview);
	main_window->show_all();
/*
	board= Gtk::manage(new Board(*this));
	box_board->pack_start(*board);

	// 设置Treeview区
	Gtk::ScrolledWindow* scrolwin=dynamic_cast<Gtk::ScrolledWindow*>
		(ui_xml->get_widget("scrolledwindow"));
	m_refTreeModel = Gtk::ListStore::create(m_columns);
	m_treeview.set_model( m_refTreeModel);
	scrolwin->add(m_treeview);
	m_treeview.append_column(_("bout"),m_columns.step_bout);
	m_treeview.append_column("  ",m_columns.player);
	m_treeview.append_column(_("step"),m_columns.step_line);
	m_treeview.set_events(Gdk::BUTTON_PRESS_MASK);
	m_treeview.signal_button_press_event().connect(sigc::mem_fun(*this,
				&MainWindow::on_treeview_click),false);

	change_status();

	Gtk::ScrolledWindow* scroll_book=dynamic_cast<Gtk::ScrolledWindow*>
		(ui_xml->get_widget("scrolledwin_book"));
	m_bookview= new BookView(this);
	scroll_book->add(*m_bookview);
    */
}
gMyqq::~gMyqq()
{
}
void gMyqq::on_tray_clicked()
{
    if(main_window->is_visible())
        hide_window();
    else
        show_window();
}
void gMyqq::show_window()
{
    //main_window->move(window_x, window_y);
    //cout<<"resume the position("<<window_x<<","<<window_y<<")"<<endl;
    main_window->show();
}
void gMyqq::hide_window()
{
    //MainWindow->get_position(window_x, window_y);
    //cout<<"save the current position ("<<window_x<<","<<window_y<<")"<<endl;
    main_window->hide();
}
void gMyqq::on_textview_enter_key_pressed()
{
}

int myqq(int argc, char** argv);

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if(!strcmp(argv[1],"--nogui"))
        {
            myqq(argc,argv);
        }
        else if(!strcmp(argv[1],"--help"))
        {
        }
        else if(!strcmp(argv[1],"--test"))
        {
            Myqq myqq(argc,argv);
        }
        else
        {
            cout<<"usage: gMystar --help for more details"<<endl;
        }
    }
    else
    {
        Gtk::Main kit(argc, argv);
        gMyqq gmyqq;
        kit.run();
    }
    return 0;
}
