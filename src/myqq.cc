#include "myqq.h"

#ifndef __WIN32__
int read_password(char *lineptr)   
{
	struct   termios   old,   new_;   
	int   nread;	
	/*   Turn   echoing   off   and   fail   if   we   can't.   */   
	if   (tcgetattr   (fileno   (stdout),   &old)   !=   0)   
		return   -1;   
	new_   =   old;   
	new_.c_lflag   &=   ~ECHO;   
	if   (tcsetattr   (fileno   (stdout),   TCSAFLUSH,   &new_)   !=   0)   
		return   -1;
	/*   Read   the   password.   */   
	nread   =   scanf   ("%31s", lineptr);	
	/*   Restore   terminal.   */   
	(void)   tcsetattr   (fileno   (stdout),   TCSAFLUSH,   &old); 	
	return   nread;   
}
#endif
/*
void buddy_msg_callback ( qqclient* qq, uint uid, time_t t, char* msg )
{
    charcolor( CCOL_LIGHTBLUE );	//red color
	char timestr[12];
	char msgstr[64];
	struct tm * timeinfo;
	//char* nick = myqq_get_buddy_name( qq, uid );
	const char* nick = myqq_get_buddy_name( qq, uid );      //--amoblin
  	timeinfo = localtime ( &t );
	strftime( timestr, 12, "%H:%M:%S", timeinfo );
	char tmp[20], *p;
	sprintf( tmp, "%-16d", uid );
	p = strstr( buddy_buf, tmp );
	if( p )
	{
		p -= 8;
		if( p>=buddy_buf )
		{
			int ln;
			sscanf( p, "%d", &ln );
			sprintf( msgstr, "\n%d)%s[", ln, timestr );
			strcat( msgstr, nick );
			strcat( msgstr, "]\n\t" );//二次修改 
			MSG("%s", _TEXT( msgstr ) );
			(msg);
			RESET_INPUT
		}
	}
    else
    {
		sprintf( msgstr, "\n%s[", timestr );
		strcat( msgstr, nick );
		strcat( msgstr, "]\n\t" );//二次修改 
		MSG("%s", _TEXT( msgstr ) );
		puts( _TEXT( msg ) );
		RESET_INPUT
	}
}
void qun_msg_callback ( qqclient* qq, uint uid, uint int_uid, time_t t, char* msg )
{
    charcolor( CCOL_REDGREEN );	//red color
	char timestr[12];
	char msgstr[64];
	const char* qun_name = myqq_get_qun_name( qq, int_uid );    //--amoblin
	const char* nick = myqq_get_qun_member_name( qq, int_uid, uid );
	struct tm * timeinfo;
  	timeinfo = localtime ( &t );
	strftime( timestr, 12, "%H:%M:%S", timeinfo );
	char tmp[20], *p;
	sprintf( tmp, "%-16d", int_uid );
	p = strstr( qun_buf, tmp );
	if( p )
	{
		p -= 8;
		if( p>=qun_buf )
		{
			int ln;
			sscanf( p, "%d", &ln );
			sprintf( msgstr, "\n%d)%s{", ln, timestr );
			strcat( msgstr, qun_name );
			strcat( msgstr, "}[" );
			strcat( msgstr, nick );
			strcat( msgstr, "]\n\t" );//二次修改
			MSG("%s", _TEXT( msgstr ) );
			puts( _TEXT( msg ) );
			RESET_INPUT
		}
	}
    else
    {
		sprintf( msgstr, "\n%s{", timestr );
		strcat( msgstr, qun_name );
		strcat( msgstr, "}[" );
		strcat( msgstr, nick );
		strcat( msgstr, "]\n\t" );//二次修改
		MSG("%s", _TEXT( msgstr ) );
		puts( _TEXT( msg ) );
		RESET_INPUT
	}
}
*/
void buddy_msg_callback ( qqclient* qq, uint uid, time_t t, char* msg )
{
    charcolor( CCOL_LIGHTBLUE );	//red color
	char timestr[12];
	char msgstr[64];
	struct tm * timeinfo;
	//char* nick = myqq_get_buddy_name( qq, uid );
	const char* nick = myqq_get_buddy_name( qq, uid );      //--amoblin
  	timeinfo = localtime ( &t );
	strftime( timestr, 12, "%H:%M:%S", timeinfo );
	char tmp[20], *p;
	sprintf( tmp, "%-16d", uid );
	p = strstr( buddy_buf, tmp );
	if( p )
	{
		p -= 8;
		if( p>=buddy_buf )
		{
			int ln;
			sscanf( p, "%d", &ln );
			sprintf( msgstr, "\n%d)%s[", ln, timestr );
			strcat( msgstr, nick );
			strcat( msgstr, "]\n\t" );//二次修改 
			//irssi->show(msgstr);
            irssi->show( _TEXT( msg ) );
		}
	}
    else
    {
		sprintf( msgstr, "\n%s[", timestr );
		strcat( msgstr, nick );
		strcat( msgstr, "]\n\t" );//二次修改 
		//MSG("%s", _TEXT( msgstr ) );
		//puts( _TEXT( msg ) );
        irssi->show(msg);
	}
}
void qun_msg_callback ( qqclient* qq, uint uid, uint int_uid, time_t t, char* msg )
{
    charcolor( CCOL_REDGREEN );	//red color
	char timestr[12];
	char msgstr[64];
	const char* qun_name = myqq_get_qun_name( qq, int_uid );    //--amoblin
	const char* nick = myqq_get_qun_member_name( qq, int_uid, uid );
	struct tm * timeinfo;
  	timeinfo = localtime ( &t );
	strftime( timestr, 12, "%H:%M:%S", timeinfo );
	char tmp[20], *p;
	sprintf( tmp, "%-16d", int_uid );
	p = strstr( qun_buf, tmp );
	if( p )
	{
		p -= 8;
		if( p>=qun_buf )
		{
			int ln;
			sscanf( p, "%d", &ln );
			sprintf( msgstr, "\n%d)%s{", ln, timestr );
			strcat( msgstr, qun_name );
			strcat( msgstr, "}[" );
			strcat( msgstr, nick );
			strcat( msgstr, "]\n\t" );//二次修改
			//MSG("%s", _TEXT( msgstr ) );
			irssi->show(msg);
		}
	}
    else
    {
		sprintf( msgstr, "\n%s{", timestr );
		strcat( msgstr, qun_name );
		strcat( msgstr, "}[" );
		strcat( msgstr, nick );
		strcat( msgstr, "]\n\t" );//二次修改
		//MSG("%s", _TEXT( msgstr ) );
		irssi->show(msg);
	}
}

Myqq::Myqq(int argc,char **argv)
{
    if(!init())
        return;
    status=0;   //初始化状态，未登录

    /*
       irssi = new Irssi;
       irssi->init(account);
       while(true)
       {
       char command[256];
       char *temp=command;
       get_command(temp);    //得到命令
       }
       endwin();
       */

    parse_command_line(argc, argv);
    /*
    else    //network error!    just for developing
    {
        curses_init();
        while(true)
        {
            char command[256];
            char *temp=command;
            get_command(temp);    //得到命令
        }
        endwin();
    }
    */
}
void Myqq::parse_command_line(int argc,char **argv)
{
    if(argc==4)
    {
        account=atoi(argv[3]);
    }
    else if(argc==2)
        checkAndSetConfig();//gmyqqrc.xml
    while(!status)
        login();
    if(status>0)
    {
        irssi = new Irssi;
        irssi->init(account);
        while(status)
        {
            char command[256];
            char *temp=command;
            get_command(temp);    //得到命令
        }
    }
}
void Myqq::set_account(uint acc)
{
    account=acc;
}
bool Myqq::login()
{
    if(account==0)
    {
        cout<<"QQ账号：";
        cin>>account;
    }
    if(*password == NULL)
    {
        cout<<"QQ密码：";
#ifdef __WIN32__
        uint pwi;
        char pswd;
        for(pwi=0;pwi<=32;pwi++)
        {
            pswd = getch(); //逐次赋值,但不回显
            if(pswd == '\x0d')//回车则终止循环
            {
                password[pwi] ='\0';//getch需要补'\0'以适合原程序 
                break;
            }
            if(pswd == '\x08')//删除键则重输QQ密码 
            {
                if( pwi>0 ) pwi=-1;
                MSG("%s",_TEXT("\n请重输QQ密码:"));
                continue;
            }
            printf("*"); //以星号代替字符
            password[pwi] =pswd;
        }
#else	//LINUX
        read_password( password );
#endif
        cout<<endl<<"是否隐身登陆？(y/n)";
        cin>>input;
        qq->mode = *input=='y' ? QQ_HIDDEN : QQ_ONLINE;
        qq->mode = QQ_HIDDEN;
    }
    qqclient_create( qq, account, password );   //包装的
    qqclient_login(qq);
    cout<<"登陆中..."<<endl;
    while( qq->process == P_LOGGING )
        qqclient_wait( qq, 1 );
    while( qq->process == P_VERIFYING )
    {
        char temp[32];
        sprintf(temp,"%d",account);
        string account_str(temp);
        string command("eog ./verify/");
        command +="*.png&";
        cout<<command.c_str();
        system(command.c_str());    //自动调出验证码图片
        cout<<"请输入验证码（验证码目录下）: ";
        cin>>input;
        qqclient_verify( qq, *(uint*)input );
        while( qq->process == P_LOGGING )
            qqclient_wait( qq, 1 );
    }
    if( qq->process != P_LOGIN )    //登录是否成功的判定依据！
    {
        switch( qq->process )
        {
            case P_ERROR:
                cout<<"网络错误.\n";
                status=-1;
                return false;
            case P_DENIED:
                cout<<"您的QQ需要激活(http://jihuo.qq.com).\n";
#ifdef __WIN32__
                ShellExecute(NULL,"open","http://jihuo.qq.com/",NULL,NULL,SW_SHOWNORMAL);
#endif
                status=-1;
                return false;
            case P_WRONGPASS:
                cout<<"您的密码错误."<<endl;
                status=-1;
                return false;
        }
        cout<<"now will logout..."<<endl;
        qqclient_logout( qq );
        qqclient_cleanup( qq );
        return false;
    }
    cout<<"success!"<<endl;
    status=1;
    return true;
}
bool Myqq::authen()
{
    if(true)    //登录失败
    {
        cout<<"登录失败！请重新登录"<<endl;
        return false;
    }
    else
        return true;
}
void Myqq::welcome_message()
{
    cout<<"欢迎使用 MyQQ2009 beta1 中文版\n"
        <<"这是一个为程序员和电脑爱好者制作的"
        <<"迷你控制台即时通讯软件,享受它吧!\n"
        <<"help:      显示帮助信息.\n"
        ;
}
void Myqq::help()
{
    irssi->show("add/a:     添加好友. add+QQ号码.\n\
            la:        list all:所有好(群)友列表.(指向前操作)\n\
            ls:        list buddies:在线好(群)友列表.(指向前操作)\n\
            lg:        list group:显示群列表.(指向前操作)\n\
            to/t:      指向某个QQ号或者前面的编号.\n\
            enter/e:   指向某个群号或者前面的编号.\n\
            leave/l:   离开群.(指向后操作)\n\
            say/s:     发送信息.(指向后操作)\n\
            info/i:    查看相应信息.(指向后操作)\n\
            update/u:  更新所有列表.\n\
            status:    改变状态(online|away|busy|killme|hidden)\n\
            verify/r:  输入验证码(验证码在verify目录下).\n\
            change/c:  更换用户登陆.\n\
            exit/quit: 退出.\n\
            h:         help:显示帮助信息.\n");
}
void Myqq::get_command(char *command)
{
    irssi->get_command(command);
    switch(command[0])
    {
        case '/':
            irssi->show_command(command);
            parse_command(command);
            break;
        case '\n':
            break;
        default:
            send_message_to_buddy(command);
    }
}
void Myqq::parse_command(char *command)
{
    //move(LINES-1,11);
    //clrtoeol();
    time_t t = time(NULL);
    struct tm *local = localtime(&t);
    char *pch = strtok(command," ");
    if(pch != NULL)
    {
        if(!strcmp(pch,"/ls"))
            list_online_buddies();
        else if(!strcmp(pch,"/la"))
            list_all();
        else if(!strcmp(pch,"/lg"))
            list_groups();
        else if(!strcmp(pch,"/t"))
        {
            pch = strtok(NULL, " ");
            if(pch!=NULL)
            {
                begin_talk_to_buddy(pch);
            }
        }
        else if(!strcmp(pch,"/status"))
            change_status();
        else if(!strcmp(pch,"/s"))
        {
            pch = strtok(NULL, " ");
            if(pch!=NULL)
            {
                send_message_to_buddy(pch);
            }
        }
        else if(!strcmp(pch,"/a"))
        {
            pch = strtok(NULL, " ");
            if(pch!=NULL)
            {
                add_buddy(pch);
            }
        }
        else if(!strcmp(pch,"/logout"))
            logout();
        else if(!strcmp(pch,"/h") || !strcmp(pch,"/help"))
            help();
        else if(!strcmp(pch,"/exit") || !strcmp(pch,"/quit"))
            quit();
        else
            error(pch);
    }

}
char *Myqq::get_online_buddies()
{
}
char *Myqq::get_all()
{
}
char *Myqq::get_groups()
{
}
void Myqq::list_online_buddies()    //'ls'
{
    myqq_get_buddy_list( qq, buddy_buf, QUN_BUF_SIZE, 1 );
    irssi->show(buddy_buf);

    myqq_get_qun_member_list( qq, qun_int_uid, buddy_buf, QUN_BUF_SIZE, 1 );
    //irssi->show(buddy_buf);
}
void Myqq::list_all()   //'la'
{
    myqq_get_qun_member_list( qq, qun_int_uid, buddy_buf, BUDDY_BUF_SIZE, 0 );
    irssi->show(buddy_buf);

    myqq_get_buddy_list( qq, buddy_buf, BUDDY_BUF_SIZE, 0 );
    irssi->show(buddy_buf);
}
void Myqq::list_groups()    //'lg'
{
    myqq_get_qun_list( qq, qun_buf, QUN_BUF_SIZE );
    irssi->show(qun_buf);
}
void Myqq::begin_talk_to_buddy(char *arg)   //'/t'
{
    enter=0;
    if( enter )//判断是否在群里
    {
        irssi->show("您在一个群中, 你可以和任何人谈话.\n");
    }
    int n = atoi( arg );
    if( n < 0xFFFF )
    {
        char *p;
        p = skip_line( buddy_buf, n-1 );
        if( p )
        {
            sscanf( p, "%u%u", &n, &buddy_number);
            irssi->show(myqq_get_buddy_name(qq, buddy_number));
        }
    }
    else
    {
        buddy_number = n;
        irssi->show(myqq_get_buddy_name(qq, buddy_number));
    }
    //sprintf( print_buf, "to: %s 没有找到.\n", arg );
    //cout<<print_buf;
}
void Myqq::send_message_to_buddy(char *arg)
{
    enter=0;
    if( enter )
    {
        /*
#ifdef	__WIN32__
        if( myqq_send_im_to_qun( qq, qun_int_uid, to_utf8(arg), 1 ) < 0 )
#else
            if( myqq_send_im_to_qun( qq, qun_int_uid, arg, 1 ) < 0 )
#endif
            {
                MSG("%s",_TEXT("超时: 您的消息发送失败.\n"));
            }
            */
    }
    else
    {
        if( buddy_number == 0 )
        {
            //MSG("%s",_TEXT("say: 和谁谈话?\n"));
        }
#ifdef	__WIN32__
        if( myqq_send_im_to_buddy( qq, buddy_number, to_utf8(arg), 1 ) < 0 )
#else
        if( myqq_send_im_to_buddy( qq, buddy_number, arg, 1 ) < 0 )
#endif
        {
            irssi->show("超时: 您的消息发送失败.");
        }
        else
        {
            irssi->show(arg);
        }
    }
}
void Myqq::change_status()
{
    /*
    //改变状态
    qqclient_change_status( qq, QQ_AWAY );
    qqclient_change_status( qq, QQ_HIDDEN );
    qqclient_change_status( qq, QQ_KILLME );
    qqclient_change_status( qq, QQ_BUSY );
    */
    qqclient_change_status( qq, QQ_ONLINE );
}
void Myqq::add_buddy(char *arg)
{
    irssi->show("添加：");
    qqclient_add( qq, atoi(arg), "");
}
void Myqq::logout()
{
    irssi->exit();
    status = 0;
    qqclient_logout( qq );
    qqclient_cleanup( qq );
    config_end();
    DEL( qq );
    MSG("%s",_TEXT("离开.\n"));
    DEL( qun_buf );
    DEL( buddy_buf );
    DEL( print_buf );
    setcolor( CCOL_NONE );
    memory_end();
}
void Myqq::quit()
{
    irssi->exit();
    cout<<"waiting for exit..."<<endl;
    status = 0;
    qqclient_logout( qq );
    cout<<"qqclient logout"<<endl;
    qqclient_cleanup( qq );
    cout<<"qqclient cleanup"<<endl;
    config_end();
    cout<<"config end..."<<endl;
    DEL( qq );
    cout<<"del qq "<<endl;
    cout<<"离开.\n";
    DEL( qun_buf );
    cout<<"del groups buffer";
    DEL( buddy_buf );
    cout<<"del buddies buffer";
    DEL( print_buf );
    cout<<"del print buffer";
    setcolor( CCOL_NONE );
    cout<<"setcolor ...";
    memory_end();
    cout<<"memory end..."<<endl;
}
void Myqq::error(char *pch)
{
    irssi->show("unknown command , type '/help' for more information");
}
Myqq::~Myqq()
{
}
bool Myqq::init()
{
    setlocale(LC_ALL,"");
    account = 0;
    *password = NULL;

    static char file[20];
    strcpy(file,getenv("HOME"));
    strcat(file,"/.gmyqq/gmyqqrc.xml");
    config_file = file;

    int cmdid, lastcmd=-1, len;
    char cmd[16], arg[1008];
    srand(time(NULL));
    //init data
    config_init();
    qqsocket_init();
    NEW( qun_buf, QUN_BUF_SIZE, char);
    NEW( buddy_buf, BUDDY_BUF_SIZE ,char);
    NEW( print_buf, PRINT_BUF_SIZE, char);
    NEW( qq, sizeof(qqclient), qqclient);


    if( !qun_buf || !buddy_buf || !print_buf || !qq )
    {
        cout<<"no enough memory.";
        return false;
    }
    return true;
}
void Myqq::buddy_msg_callback ( qqclient* qq, uint uid, time_t t, char* msg )
{
    charcolor( CCOL_LIGHTBLUE );	//red color
	char timestr[12];
	char msgstr[64];
	struct tm * timeinfo;
	const char* nick = myqq_get_buddy_name( qq, uid );      //--amoblin
  	timeinfo = localtime ( &t );
	strftime( timestr, 12, "%H:%M:%S", timeinfo );
	char tmp[20], *p;
	sprintf( tmp, "%-16d", uid );
	p = strstr( buddy_buf, tmp );
	if( p )
	{
		p -= 8;
		if( p>=buddy_buf )
		{
			int ln;
			sscanf( p, "%d", &ln );
			sprintf( msgstr, "\n%d)%s[", ln, timestr );
			strcat( msgstr, nick );
			strcat( msgstr, "]\n\t" );//二次修改 
			irssi->show(msgstr);
		}
	}
    else
    {
		sprintf( msgstr, "\n%s[", timestr );
		strcat( msgstr, nick );
		strcat( msgstr, "]\n\t" );//二次修改 
        irssi->show(msgstr);
	}
}
void Myqq::qun_msg_callback ( qqclient* qq, uint uid, uint int_uid, time_t t, char* msg )
{
    charcolor( CCOL_REDGREEN );	//red color
	char timestr[12];
	char msgstr[64];
	const char* qun_name = myqq_get_qun_name( qq, int_uid );    //--amoblin
	const char* nick = myqq_get_qun_member_name( qq, int_uid, uid );
	struct tm * timeinfo;
  	timeinfo = localtime ( &t );
	strftime( timestr, 12, "%H:%M:%S", timeinfo );
	char tmp[20], *p;
	sprintf( tmp, "%-16d", int_uid );
	p = strstr( qun_buf, tmp );
	if( p )
	{
		p -= 8;
		if( p>=qun_buf )
		{
			int ln;
			sscanf( p, "%d", &ln );
			sprintf( msgstr, "\n%d)%s{", ln, timestr );
			strcat( msgstr, qun_name );
			strcat( msgstr, "}[" );
			strcat( msgstr, nick );
			strcat( msgstr, "]\n\t" );//二次修改
			MSG("%s", _TEXT( msgstr ) );
			puts( _TEXT( msg ) );
		}
	}
    else
    {
		sprintf( msgstr, "\n%s{", timestr );
		strcat( msgstr, qun_name );
		strcat( msgstr, "}[" );
		strcat( msgstr, nick );
		strcat( msgstr, "]\n\t" );//二次修改
		MSG("%s", _TEXT( msgstr ) );
		puts( _TEXT( msg ) );
	}
}

Irssi::Irssi()
{
    //PANEL *my_panel;

    initscr();
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    cbreak();
    nonl();
    noecho();
    title = newwin(1,COLS,0,0);
    textarea = newwin(LINES-3,COLS,1,0);
    statusbar = newwin(1,COLS,LINES-2,0);
    commandline = newwin(1,COLS,LINES-1,0);

    wbkgd(title,COLOR_PAIR(1));
    wbkgd(textarea,COLOR_PAIR(2));
    wbkgd(statusbar,COLOR_PAIR(1));
    wbkgd(commandline,COLOR_PAIR(2));

    //wrefresh(title);                
    //wrefresh(statusbar);                
    //wrefresh(textarea);                
    //wrefresh(commandline);                
    refresh();

    intrflush(stdscr, FALSE);
    keypad(commandline, TRUE);
    cbreak();

}
void Irssi::show(const char *str)
{
    wmove(textarea,1,0);
    wclrtobot(textarea);
    time_t t = time(NULL);
    struct tm *local = localtime(&t);
    getyx(textarea,row,col);
    mvwprintw(textarea,1,0,"%s",str);
    wrefresh(textarea);
    move(LINES-1,11);
}
void Irssi::show_int(uint str)
{
    time_t t = time(NULL);
    struct tm *local = localtime(&t);
    getyx(textarea,row,col);
    mvwprintw(textarea,1,0,"%d",str);
    wrefresh(textarea);
}
void Irssi::show_command(char *str)
{
    time_t t = time(NULL);
    struct tm *local = localtime(&t);
    getyx(textarea,row,col);
    mvwprintw(textarea,0,0,"%2d:%2d %s",local->tm_hour,local->tm_min,str);
    wrefresh(textarea);
}
void Irssi::init(uint account)
{
    char message[COLS];
    time_t t = time(NULL);
    struct tm *local = localtime(&t);
    mvwprintw(title,0,COLS/2-10,"Welcome to gmyqq");
    wprintw(statusbar,"[%2d:%2d] [%d] [1]",local->tm_hour,local->tm_min,account);
    wprintw(commandline,"[(status)]");
    
    wrefresh(title);                
    wrefresh(statusbar);                
    wrefresh(textarea);                
    wrefresh(commandline);                
    refresh();
    row=col=0;
}
void Irssi::get_command(char *command_str)
{
    mvwprintw(commandline,0,0,"[(status)] ");       
    wclrtoeol(commandline);
    move(LINES-1,11);
    int posi=11;
    int c;
    int i=0;
    while(true)
    {
        c=getch();
        if (c == 27)
        {
            wmove(commandline,0,11);
            wclrtoeol(commandline);
            wrefresh(commandline);
            break;
        }
        else if (c == 13) //回车键执行命令
        {
            int n = len(0);
            int i;
            for(i=0;i<n;i++)
            {
                command_str[i] = mvwinch(commandline,0,i+11) & A_CHARTEXT;
            }
            command_str[n]='\0';    //很重要！否则会有乱码

            wmove(commandline,0,11);
            wclrtoeol(commandline);
            wrefresh(commandline);
            break;
        }
        else if(c == 127)
        {
            if(posi>11)//防越界
            {
                posi--;
                mvwdelch(commandline,0,posi);
                wrefresh(commandline);
            }
        }
        else
        {
            if(posi<COLS-1)
            {
                waddch(commandline,c);
                wrefresh(commandline);
                posi++;
            }
        }
    }
}
int Irssi::len(int lineno)
{
    int linelen = COLS - 1;

    while (linelen >= 0 && mvwinch(commandline,lineno, linelen) == ' ')
        linelen--;
    return linelen + 1;
}
void Irssi::exit()
{
    delwin(title);
    delwin(textarea);
    delwin(statusbar);
    delwin(commandline);
    endwin();
}

int myqq(int argc, char** argv)
{
    //-----init----
    int cmdid, lastcmd=-1, len;
    char cmd[16], arg[1008];
    srand(time(NULL));
    //init data
    config_init();
    qqsocket_init();
    NEW( qun_buf, QUN_BUF_SIZE, char);
    NEW( buddy_buf, BUDDY_BUF_SIZE ,char);
    NEW( print_buf, PRINT_BUF_SIZE, char);
    NEW( qq, sizeof(qqclient), qqclient);

    if( !qun_buf || !buddy_buf || !print_buf || !qq )
    {
        MSG("%s","no enough memory.");
        return -1;
    }
    //login
lab:if(argc<3)
    {
        uint uid;
        //uid=USERNAME;
        char password[32];
        MSG("%s",_TEXT("QQ账号:"));
        scanf("%u", &uid );
        MSG("%s",_TEXT("QQ密码:"));
#ifdef __WIN32__
        uint pwi;
        char pswd;
        for(pwi=0;pwi<=32;pwi++)
        {
            pswd = getch(); //逐次赋值,但不回显
            if(pswd == '\x0d')//回车则终止循环
            {
                password[pwi] ='\0';//getch需要补'\0'以适合原程序 
                break;
            }
            if(pswd == '\x08')//删除键则重输QQ密码 
            {
                if( pwi>0 ) pwi=-1;
                MSG("%s",_TEXT("\n请重输QQ密码:"));
                continue;
            }
            printf("*"); //以星号代替字符
            password[pwi] =pswd;
        }
#else	//LINUX
        read_password( password );
#endif
        MSG("%s",_TEXT("\n是否隐身登陆？(y/n)"));
        scanf("%s", input );
        qqclient_create( qq, uid, password );
        qq->mode = *input=='y' ? QQ_HIDDEN : QQ_ONLINE;
        qqclient_login( qq );
        scanf("%c", input ); //If I don't add this, it will display '>' twice.
    }
    else
    {
        qqclient_create( qq, atoi(argv[1]), argv[2] );
        if( argc > 3 )
            qq->mode = atoi(argv[3])!=0 ? QQ_HIDDEN : QQ_ONLINE;
        qqclient_login( qq );
    }
    MSG("%s",_TEXT("登陆中...\n"));
    while( qq->process == P_LOGGING )
        qqclient_wait( qq, 1 );
    while( qq->process == P_VERIFYING )
    {
        string command("eog ./verify/");
        command +="*.png&";
        cout<<command.c_str();
        system(command.c_str());    //自动调出验证码图片
        MSG("%s",_TEXT("请输入验证码（验证码目录下）: "));
        scanf( "%s", input );
        qqclient_verify( qq, *(uint*)input );
        while( qq->process == P_LOGGING )
            qqclient_wait( qq, 1 );
    }
    if( qq->process != P_LOGIN )    //登录是否成功的判定依据！
    {
        switch( qq->process )
        {
            case P_ERROR:
                MSG("%s",_TEXT("网络错误.\n"));
                goto lab;
            case P_DENIED:
                MSG("%s",_TEXT("您的QQ需要激活(http://jihuo.qq.com).\n"));
#ifdef __WIN32__
                ShellExecute(NULL,"open","http://jihuo.qq.com/",NULL,NULL,SW_SHOWNORMAL);
#endif
                goto lab;
            case P_WRONGPASS:
                MSG("%s",_TEXT("您的密码错误.\n"));
                goto lab;
        }
        qqclient_logout( qq );
        qqclient_cleanup( qq );
        return 0;
    }
    //登录成功！
    MSG("%s", _TEXT(help_msg) );
    while( qq->process != P_INIT )
    {
        RESET_INPUT
            len = _getline( input, 1023 );
        if( len < 1 ) continue;
        char* sp = strchr( input, ' ' );
        if( sp )
        {
            *sp = '\0';
            strncpy( cmd, input, 16-1 );
            strncpy( arg, sp+1, 1008-1 );
            *sp = ' ';
        }
        else
        {
            strncpy( cmd, input, 16-1 );
            arg[0] = '\0';
        }
        need_reset = 1;
        for( cmdid=0; cmdid<sizeof(commands)/16; cmdid++ )
            if( strcmp( commands[cmdid], cmd )==0 )
                break;
SELECT_CMD:
        switch( cmdid )
        {
            case CMD_TO:
            case CMD_TO2:
                {
                    if( enter )
                    {
                        MSG("%s",_TEXT("您在一个群中, 你可以和任何人谈话.\n"));
                        break;
                    }
                    int n = atoi( arg );
                    if( n < 0xFFFF )
                    {
                        char *p;
                        p = skip_line( buddy_buf, n-1 );
                        if( p )
                        {
                            sscanf( p, "%u%u", &n, &to_uid );
                            sprintf( print_buf, "您将和 %s 进行谈话\n", myqq_get_buddy_name(qq, to_uid) );
                            MSG("%s", _TEXT(print_buf) );
                            break;
                        }
                    }
                    else
                    {
                        to_uid = n;
                        sprintf( print_buf, "您将和 %s 进行谈话\n", myqq_get_buddy_name(qq, to_uid) );
                        MSG("%s", _TEXT(print_buf) );
                        break;
                    }
                    sprintf( print_buf, "to: %s 没有找到.\n", arg );
                    MSG("%s", _TEXT(print_buf) );
                    break;
                }
            case CMD_SAY:
            case CMD_SAY2:
                {
                    if( enter )
                    {
#ifdef	__WIN32__
                        if( myqq_send_im_to_qun( qq, qun_int_uid, to_utf8(arg), 1 ) < 0 )
#else
                        if( myqq_send_im_to_qun( qq, qun_int_uid, arg, 1 ) < 0 )
#endif
                        {
                            MSG("%s",_TEXT("超时: 您的消息发送失败.\n"));
                        }
                    }
                    else
                    {
                        if( to_uid == 0 )
                        {
                            MSG("%s",_TEXT("say: 和谁谈话?\n"));
                            break;
                        }
#ifdef	__WIN32__
                        if( myqq_send_im_to_buddy( qq, to_uid, to_utf8(arg), 1 ) < 0 )
#else
                        if( myqq_send_im_to_buddy( qq, to_uid, arg, 1 ) < 0 )
#endif
                            {
                                MSG("%s",_TEXT("超时: 您的消息发送失败.\n"));
                            }
                    }
                    break;
                }
            case CMD_EXIT:
            case CMD_EXIT2:
                //case CMD_EXIT3:
                goto end;
            case CMD_HELP:
                MSG("%s", _TEXT(help_msg) );
                break;
            case CMD_STATUS:
                if( strcmp( arg, "away") == 0 )
                    qqclient_change_status( qq, QQ_AWAY );
                else if( strcmp( arg, "online") == 0 )
                    qqclient_change_status( qq, QQ_ONLINE );
                else if( strcmp( arg, "hidden") == 0 )
                    qqclient_change_status( qq, QQ_HIDDEN );
                else if( strcmp( arg, "killme") == 0 )
                    qqclient_change_status( qq, QQ_KILLME );
                else if( strcmp( arg, "busy") == 0 )
                    qqclient_change_status( qq, QQ_BUSY );
                else
                {
                    MSG("%s",_TEXT("未知状态\n") );
                }
                break;
            case CMD_ENTER:
            case CMD_ENTER2:
                {
                    int n = atoi( arg );
                    if( n < 0xFFFF )
                    {
                        char *p;
                        p = skip_line( qun_buf, n-1 );
                        if( p )
                        {
                            sscanf( p, "%u%u", &n, &qun_int_uid );
                            sprintf( print_buf, "您在 %s 群中\n", myqq_get_qun_name( qq, qun_int_uid) );
                            MSG("%s", _TEXT(print_buf) );
                            enter = 1;
                            break;
                        }
                    }
                    else
                    {
                        qun_int_uid = n;
                        sprintf( print_buf, "您在 %s 群中\n", myqq_get_qun_name( qq, qun_int_uid) );
                        MSG("%s", _TEXT(print_buf) );
                        enter = 1;
                        break;
                    }
                    sprintf( print_buf, "enter: %s 没有找到.\n", arg );
                    MSG("%s", _TEXT(print_buf) );
                    break;
                }
            case CMD_LEAVE:
            case CMD_LEAVE2:
                if( !enter )
                {
                    MSG("%s",_TEXT("您没有进入群.\n"));
                    break;
                }
                enter = 0;
                sprintf( print_buf, "离开 %s. 您将和 %s 进行谈话\n", 
                        myqq_get_qun_name( qq, qun_int_uid ), myqq_get_buddy_name( qq, to_uid ) );
                MSG("%s", _TEXT(print_buf) );
                break;
            case CMD_QUN:
            case CMD_QUN2:
                {
                    myqq_get_qun_list( qq, qun_buf, QUN_BUF_SIZE );
                    MSG("%s", _TEXT( qun_buf ) );
                    break;
                }
            case CMD_UPDATE:
            case CMD_UPDATE2:
                qun_update_all( qq );
                buddy_update_list( qq );
                group_update_list( qq );
                MSG("%s",_TEXT("更新中...\n"));
                if( qqclient_wait( qq, 20 )<0 )
                {
                    MSG("%s",_TEXT("更新超时.\n"));
                }
                break;
            case CMD_INFO:
            case CMD_INFO2:
                {
                    if( !enter )
                    {
                        if( to_uid==0 )
                        {
                            MSG("%s",_TEXT("请先选择一个好友.\n"));
                        }
                        else
                        {
                            char* buf = (char*)malloc(1024*4);	//4kb enough!
                            if( myqq_get_buddy_info( qq, to_uid, buf, 1024*4 ) < 0 )
                            {
                                sprintf( print_buf, "获取 %s 的信息失败\n", myqq_get_buddy_name( qq, to_uid ) );
                                MSG("%s", _TEXT(print_buf) );
                            }
                            else
                            {
                                MSG("%s", _TEXT( buf ) );
                            }
                            free(buf);
                        }
                    }
                    else
                    {
                        char* buf = (char*)malloc(1024*4);	//4kb enough!
                        if( myqq_get_qun_info( qq, qun_int_uid, buf, 1024*4 ) < 0 )
                        {
                            sprintf( print_buf, "获取 %s 的信息失败\n", myqq_get_qun_name( qq, qun_int_uid ) );
                            MSG("%s", _TEXT(print_buf) );
                        }
                        else
                        {
                            MSG( "%s",_TEXT( buf ) );
                        }
                        free(buf);
                    }
                    break;
                }
            case CMD_VIEW:
            case CMD_VIEW2:
                if( enter ) //'lg'
                {
                    myqq_get_qun_member_list( qq, qun_int_uid, buddy_buf,
                            BUDDY_BUF_SIZE, 0 );
                    MSG("%s", _TEXT( buddy_buf ) );
                }
                else    //'la'
                {
                    myqq_get_buddy_list( qq, buddy_buf, BUDDY_BUF_SIZE, 0 );
                    MSG("%s", _TEXT( buddy_buf ) );
                }
                break;
            case CMD_WHO:
            case CMD_WHO2:
                if( enter )
                {
                    myqq_get_qun_member_list( qq, qun_int_uid, buddy_buf,
                            QUN_BUF_SIZE, 1 );
                    MSG("%s", _TEXT( buddy_buf ) );
                }
                else
                {
                    myqq_get_buddy_list( qq, buddy_buf, QUN_BUF_SIZE, 1 );
                    MSG("%s", _TEXT( buddy_buf ) );
                }
                break;
            case CMD_CHANGE:
            case CMD_CHANGE2:
                qqclient_logout( qq );
                qqclient_cleanup( qq );
                myqq( 0, NULL );
                goto end;
            case CMD_VERIFY:
            case CMD_VERIFY2:
                qqclient_verify( qq, *((uint*)arg) );
                break;
            case CMD_ADD:
            case CMD_ADD2:
                {
                    sprintf( print_buf, "添加[%d]的附言（默认空）：", atoi(arg) );
                    MSG("%s", _TEXT(print_buf) );
                    _getline( input, 50 );
                    qqclient_add( qq, atoi(arg), input );
                    break;
                }
            case CMD_DEL:
                qqclient_del( qq, atoi(arg) );
                break;
            default:
                //use it as the last cmd's argument
                if( lastcmd && *input )
                {
                    cmdid = lastcmd;
                    strncpy( arg, input, 1008-1 );
                    *input = 0;
                    goto SELECT_CMD;
                }
                break;
        }
        lastcmd = cmdid;
    }
end:
    qqclient_logout( qq );
    qqclient_cleanup( qq );
    config_end();
    DEL( qq );
    MSG("%s",_TEXT("离开.\n"));
    DEL( qun_buf );
    DEL( buddy_buf );
    DEL( print_buf );
    setcolor( CCOL_NONE );
    memory_end();
    return 0;
}
