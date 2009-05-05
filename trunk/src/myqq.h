/*
 *  myqq.h  ------come from myqq.c,coverted by amoblin
 *
 *  main() A small console for MyQQ.
 *
 *  Copyright (C) 2008  Huang Guan (gdxxhg@gmail.com)
 *
 *  2008-01-31 Created.
 *  2008-2-5   Add more commands.
 *  2008-7-15  Mr. Wugui said "There's no accident in the world.", 
 *	       but I always see accident in my code :)
 *  2008-10-1  Character color support and nonecho password.
 *  2009-1-25  Use UTF-8 as a default type.
 *
 *  Description: This file mainly includes the functions about 
 *               Parsing Input Commands.
 *		 Warning: this file should be in UTF-8.
 *               
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#ifdef __WIN32__
#include <conio.h>
#include <winsock.h>
#include <wininet.h>
#include <windows.h>

#else
#include <termios.h> //read password
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
//amoblin
#include <ncurses.h>     //curses中包含stdio.h
#include <panel.h>
#include <string>
#include <time.h>
#include <sys/stat.h>
#endif

extern "C"
{
#include "qqclient.h"
#include "buddy.h"
#include "qun.h"
#include "group.h"
#include "memory.h"
#include "utf8.h"
#include "config.h"
#include "qqsocket.h"
}

using namespace std;

#define MSG need_reset = 1; setcolor( color_index ); printf

#define QUN_BUF_SIZE	80*100
#define BUDDY_BUF_SIZE	80*500
#define PRINT_BUF_SIZE	80*500*3


static char* qun_buf, *buddy_buf, *print_buf;
static uint to_uid = 0;		//send message to that guy.
static uint qun_int_uid;		//internal qun id if entered.
static char input[1024];
static int enter = 0;
static qqclient* qq;
static int need_reset;
enum
{
	CMD_EXIT = 0, CMD_EXIT2,//CMD_EXIT3,
	CMD_SAY, CMD_SAY2,
	CMD_TO, CMD_TO2,
	CMD_HELP,
	CMD_STATUS,
	CMD_ENTER, CMD_ENTER2,
	CMD_LEAVE, CMD_LEAVE2,
	CMD_WHO, CMD_WHO2,
	CMD_VIEW, CMD_VIEW2,
	CMD_QUN, CMD_QUN2,
	CMD_INFO, CMD_INFO2,
	CMD_UPDATE, CMD_UPDATE2,
	CMD_CHANGE, CMD_CHANGE2,
	CMD_TEST,
	CMD_VERIFY, CMD_VERIFY2,
	CMD_ADD, CMD_ADD2,
	CMD_DEL
};
static char commands[][16]=
{
	"exit", "q",//"quit",
	"say", "s",
	"to", "t",  //talk with buddy.
	"help",
	"status",
	"enter", "e",   //
	"leave", "l",
	"list", "ls",   //who
	"listall", "la",    //view
	"listgroup", "lg",  //qun
	"info", "i",
	"update", "u",
	"change", "c",
	"test",
	"verify", "r",
	"add","a",
	"del"
};

static char help_msg[]=
"欢迎使用 MyQQ2009 beta1 中文版\n"
"这是一个为程序员和电脑爱好者制作的"
"迷你控制台即时通讯软件,享受它吧!\n"
"help:      显示以下帮助信息.\n"
"add/a:       添加好友. add+QQ号码.\n"
"listall/la:    所有好(群)友列表.(指向前操作)\n"
"list/ls:     在线好(群)友列表.(指向前操作)\n"
"listgroup/lg:     显示群列表.(指向前操作)\n"
"to/t:      指向某个QQ号或者前面的编号.\n"
"enter/e:   指向某个群号或者前面的编号.\n"
"leave/l:   离开群.(指向后操作)\n"
"say/s:     发送信息.(指向后操作)\n"
"info/i:    查看相应信息.(指向后操作)\n"
"update/u:  更新所有列表.\n"
"status:    改变状态(online|away|busy|killme|hidden)\n"
"verify/r:  输入验证码(验证码在verify目录下).\n"
"change/c:  更换用户登陆.\n"
"exit/q:    退出.\n"
;

#ifdef __WIN32__
#define CCOL_GREEN	FOREGROUND_GREEN
#define CCOL_LIGHTBLUE	FOREGROUND_BLUE | FOREGROUND_GREEN
#define CCOL_REDGREEN	FOREGROUND_RED | FOREGROUND_GREEN
#define CCOL_NONE		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
static int color_index = CCOL_NONE; 
static void charcolor( int col )
{
	color_index = col;
}
static void setcolor( int col )
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY | col);
}
#else

#define CCOL_NONE		"\033[0m"   
#define CCOL_BLACK	"\033[0;30m"   
#define CCOL_DARKGRAY	"\033[1;30m"   
#define CCOL_BLUE		"\033[0;34m"   
#define CCOL_LIGHTBLUE	"\033[1;34m"   
#define CCOL_GREEN	"\033[0;32m"   
#define CCOL_LIGHTGREEN	"\033[1;32m"   
#define CCOL_CYAN		"\033[0;36m"   
#define CCOL_LIGHTCYAN	"\033[1;36m"   
#define CCOL_RED		"\033[0;31m"   
#define CCOL_LIGHTRED	"\033[1;31m"   
#define CCOL_PURPLE	"\033[0;35m"   
#define CCOL_LIGHTPURPLE	"\033[1;35m"   
#define CCOL_LIGHTBROWN	"\033[0;33m"   
#define CCOL_YELLOW	"\033[1;33m"   
#define CCOL_LIGHTGRAY	"\033[0;37m"   
#define CCOL_WHITE	"\033[1;37m"
#define CCOL_REDGREEN	CCOL_YELLOW
//static char* color_index = CCOL_NONE;
const static char* color_index = CCOL_NONE; //--amoblin
static void charcolor( const char* col )
{
	color_index = (char*)col;
}
static void setcolor( const char* col )
{
	printf("%s", col );
}
#endif


//含颜色控制 
#define RESET_INPUT \
	if( need_reset )\
	{	charcolor( CCOL_NONE );\
	if( enter ){ \
		MSG("In {%s}> ", _TEXT( myqq_get_qun_name( qq, qun_int_uid ) ) ); \
	}else{ \
		MSG("To [%s]> ", _TEXT( myqq_get_buddy_name( qq, to_uid ) ) );} \
	fflush( stdout ); \
	need_reset = 0;\
    }


#ifdef __WIN32__
#define _TEXT to_gb_force 
static char* to_gb_force( char* s )
{
    //	memset( print_buf, 0, PRINT_BUF_SIZE );
	utf8_to_gb( s, print_buf, PRINT_BUF_SIZE-1 );
	return print_buf;
}
static char* to_utf8( char* s )
{
    //	memset( print_buf, 0, PRINT_BUF_SIZE );
	gb_to_utf8( s, print_buf, PRINT_BUF_SIZE-1 );
	return print_buf;
}

#else
#define _TEXT
#endif





static int _getline(char *s, int lim)
{
    char *t;
    int c;

    t = s;
    while (--lim>1 && (c=getchar()) != EOF && c != '\n')
        *s++ = c;
    *s = '\0';
    return s - t;
}

//static char* mode_string( int mode )
const static char* mode_string( int mode )      //--amoblin
{
	switch( mode )
    {
        case QQ_ONLINE:
            return "Online";
        case QQ_AWAY:
            return "Away";
        case QQ_BUSY:
            return "Busy";
        case QQ_OFFLINE:
            return "Offline";
        case QQ_HIDDEN:
            return "Hidden";
        case QQ_KILLME:
            return "Kill Me";
        case QQ_QUIET:
            return "Quiet";
    }
    return "Unknown";
}
static char* skip_line( char* p, int ln )
{
	while( *p && ln-- )
	{
		p ++;
		while( *p && *p!='\n' ) p++;
	}
	return p;
}
//static char* myqq_get_buddy_name( qqclient* qq, uint uid )
const static char* myqq_get_buddy_name( qqclient* qq, uint uid )    //--amoblin
{
	static char tmp[16];
	qqbuddy* b = buddy_get( qq, uid, 0 );
	if( b )
		return b->nickname;
	if( uid == 10000 )
		return "System";
	if( uid != 0 )
    {
		sprintf( tmp, "%u" , uid );
		return tmp;
	}
	return "Nobody";
}
const static char* myqq_get_qun_name( qqclient* qq, uint uid )      //--amoblin
{
	static char tmp[16];
	qqqun* q = qun_get( qq, uid, 0 );
	if( q )
		return q->name;
	if( uid != 0 )
    {
		sprintf( tmp, "%u" , uid );
		return tmp;
	}
	return "[q==NULL]";
}
const static char* myqq_get_qun_member_name( qqclient* qq, uint int_uid, uint uid ) //-amoblin
{
	static char tmp[16];
	qqqun* q = qun_get( qq, int_uid, 0 );
	if( q )
    {
		qunmember* m = qun_member_get( qq, q, uid, 0 );
		if( m )
			return m->nickname;
		if( uid != 0 )
        {
			sprintf( tmp, "%u" , uid );
			return tmp;
		}
		return "[m==NULL]";
	}
	return "[q==NULL]";
}
static int myqq_send_im_to_qun( qqclient* qq, uint int_uid, char* msg, int wait )
{
	qun_send_message( qq, int_uid, msg );
	if( wait )
	{
		if( qqclient_wait( qq, 15 ) < 0 )
			return -1;
	}
	return 0;
}
static int myqq_send_im_to_buddy( qqclient* qq, uint int_uid, char* msg, int wait )
{
	buddy_send_message( qq, int_uid, msg );
	if( wait )
	{
		if( qqclient_wait( qq, 15 ) < 0 )
			return -1;
	}
	return 0;
}
static int myqq_get_buddy_info( qqclient* qq, uint uid, char* buf, int size )
{
	qqbuddy *b = buddy_get( qq, uid, 0 );
	if( size < 256 )
		return -1;
	if( b == NULL )
		return -2;
	int len, ip = htonl(b->ip);
	len = sprintf( buf,     "好友昵称\t%-32s\n"
				"用户账号\t%-32d\n"
				"IP地址\t\t%-32s\n"
				"IP端口\t\t%-32d\n"
				"头像\t\t%-32d\n"
				"年龄\t\t%-32d\n"
				"性别\t\t%-32s\n"
				"状态\t\t%-32s\n",
		b->nickname, b->number, inet_ntoa( *(struct in_addr*)&ip ), b->port, b->face, b->age,
		(b->sex==0x00)?"Male": (b->sex==0x01)?"Female":"Asexual", mode_string(b->status) );
	return len;
}



//Note: this function change the source string directly.
static char* util_escape( char* str )
{
	unsigned char* ch;
	ch = (unsigned char*)str;
	while( *ch )
	{
		if( *ch < 0x80 ) //ASCII??
		{
			if( !isprint(*ch) ) //if not printable??
				*ch = ' ';	//use space instead..
			ch ++;	//skip one
		}
        else
        {
			ch +=2; //skip two
		}
	}
	return str;
}

/*   The output buf looks like this:
L8bit  L16bit        L16bit        L32bit
1      357339036     online		Xiao xia
2      273310179     offline	Huang Guan
*/
//Note: size must be big enough
static int myqq_get_buddy_list( qqclient* qq, char* buf, int size, char online )
{
	int i, len = 0;
	//avoid editing the array
	buf[0] = 0;
	pthread_mutex_lock( &qq->buddy_list.mutex );
	int ln = 1;
	for( i=0; i<qq->buddy_list.count; i++ )
	{
		qqbuddy * b = (qqbuddy*)qq->buddy_list.items[i];
		if( online && b->status == QQ_OFFLINE ) continue;
		len = sprintf( buf, "%s%-8d%-16d%-16s%-16.64s\n", buf, ln ++, b->number, 
			mode_string( (int) b->status ), util_escape( b->nickname ) );
		if( len + 80 > size ) break;
	}
	pthread_mutex_unlock( &qq->buddy_list.mutex );
	return len;
}

/*   The output buf looks like this:
L8bit  L16bit        L16bit        L32bit
1      467788923     12118724	Xinxing Experimental School
2      489234223     13223423	SGOS2007
*/
//Note: size must be big enough
static int myqq_get_qun_list( qqclient* qq, char* buf, int size )
{
	int i, len = 0, ln=1;
	//avoid editing the array
	buf[0] = 0;
	pthread_mutex_lock( &qq->qun_list.mutex );
	for( i=0; i<qq->qun_list.count; i++ )
	{
		qqqun * q = (qqqun *)qq->qun_list.items[i];
		len = sprintf( buf, "%s%-8d%-16d%-16d%-16.64s\n", buf, ln ++, q->number, 
			q->ext_number, util_escape( q->name ) );
		if( len + 80 > size ) break;
	}
	pthread_mutex_unlock( &qq->qun_list.mutex );
	return len;
}

/*   The output buf looks like this:
L8bit  L16bit        L16bit        L32bit
1      357339036     Manager	Xiaoxia
2      273310179     Fellow		Huang Guan
*/
//Note: size must be big enough
static int myqq_get_qun_member_list( qqclient* qq, uint int_uid, char* buf, int size, char online )
{
	qqqun * q = qun_get( qq, int_uid, 0 );
	if( !q )return 0;
	//Hope the Qun won't get removed while we are using it. 
	int i, len = 0, ln = 1;
	buf[0] = 0;
	pthread_mutex_lock( &q->member_list.mutex );
	for( i=0; i<q->member_list.count; i++ )
	{
		qunmember * m = (qunmember *)q->member_list.items[i];
		if( online && m->status == QQ_OFFLINE ) continue;
		len = sprintf( buf, "%s%-8d%-16d%-16s%-16.64s\n", buf, ln++, m->number, 
			(m->role&1)?"Admin":"Fellow", util_escape( m->nickname ) );
		if( len + 80 > size )
			break;
	}
	pthread_mutex_unlock( &q->member_list.mutex );
	return len;
}

//interface for getting qun information
/* Output style:
*/
static int myqq_get_qun_info( qqclient* qq, uint int_uid, char* buf, int size )
{

	char cate_str[4][10] = {"Classmate", "Friend", "Workmate", "Other" };
	qqqun *q = qun_get( qq, int_uid, 0 );
	if( !q )return 0;
	int len;
	if( size < 256 )
		return -1;

	if( q == NULL )
		return -2;
	len = sprintf( buf, 	"名称\t\t%-32s\n"
				"内部号码\t%-32d\n"
				"群号码\t\t%-32d\n"
				"群类型\t\t%-32s\n"
				"是否允许加入\t%-32s\n"
				"群分类\t\t%-32s\n"
				"创建人\t\t%-32d\n"
				"成员数量\t%-32d\n"
				"群的简介\n%-32s\n"
				"群的公告\n%-32s\n",
		q->name, q->number, q->ext_number, (q->type==0x01)?"Normal":"Special",
		(q->auth_type==0x01)?"No": (q->auth_type==0x02)?"Yes":
			(q->auth_type==0x03)?"RejectAll":"Unknown",
		q->category > 3 ? cate_str[3] : cate_str[(int)q->category],
		q->owner, q->member_list.count, q->intro, q->ann );
	return len;
}

/*
//含颜色控制
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
			puts( _TEXT( msg ) );
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
*/

/*
//含颜色控制
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

#ifndef __WIN32__
//int read_password(char*);
#endif


class Irssi
{
    public:
        Irssi();
        ~Irssi();
        void set_statusbar(char *);
        void show(const char *);
        void show_command(char *);
        void show_int(uint);
        void init(uint);
        void get_command(char *);
        int len(int);
        void exit();
    private:
        WINDOW *title;
        WINDOW *textarea;
        WINDOW *statusbar;
        WINDOW *commandline;
        int row,col;
};
class Myqq
{
    public:
        Myqq(int ,char **);
        ~Myqq();
        void set_account(uint acc);
    protected:
        bool login();    //登录
        void welcome_message();
        void help();    //命令帮助
        bool authen();  //登录验证
        void get_command(char *);  //取得输入命令
        void parse_command(char *);   //分析命令并执行

        bool init();
        void parse_command_line(int , char**);


        void checkAndSetConfig();
        void init_file();
        int save_file();

        char *get_online_buddies();
        char *get_all();
        char *get_groups();
        void list_online_buddies();
        void list_all();
        void list_groups();
        void begin_talk_to_buddy(char *);
        void send_message_to_buddy(char *);
        void change_status();
        void add_buddy(char *);
        void logout();
        bool send_message(int);
        void quit();
        void error(char *);
        void qun_msg_callback (qqclient* qq, uint uid, uint int_uid, time_t t, char* msg );
        void buddy_msg_callback ( qqclient* qq, uint uid, time_t t, char* msg );


        qqclient *qq;
    private:
        //char account[32];   //用户名
        uint account;   //用户名
        char password[32];  //密码
        int status; //状态：0,未登录;1,在线，2,隐身，3,busy，4...
        char *config_file;
        WINDOW *qq_window[4];
        uint buddy_number;
};
