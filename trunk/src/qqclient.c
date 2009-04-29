/*
 *  qqclient.c
 *
 *  QQ Client. 
 *
 *  Copyright (C) 2008  Huang Guan
 *
 *  2008-7-12 Created.
 *  2008-10-26 TCP UDP Server Infos are loaded from configuration file.
 *
 *  Description: This file mainly includes the functions about 
 *  loading configuration, connecting server, guard thread, basic interface
 *
 */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __WIN32__
#include <winsock.h>
#include <wininet.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include "md5.h"
#include "memory.h"
#include "debug.h"
#include "config.h"
#include "qqsocket.h"
#include "packetmgr.h"
#include "protocol.h"
#include "qqclient.h"
#include "qun.h"
#include "group.h"
#include "buddy.h"
#include "util.h"


static server_item tcp_servers[MAX_SERVER_ADDR];
static server_item udp_servers[MAX_SERVER_ADDR];
static int tcp_server_count = 0, udp_server_count = 0;

static uint last_server_ip = 0, last_server_port = 0;	//for quick login
//static uchar last_server_info[15];

static void read_server_addr( server_item* srv, char* s, int* count  )
{
	char ip[32], port[10], read_name = 1, *p;
	int j = 0;
	for( p=s; ; p++ ){
		if( *p == ':' ){
			ip[j]=0;
			j=0; read_name = 0;
		}else if( *p=='|' || *p=='\0' ){
			port[j]=0;
			j=0; read_name = 1;
			if( *count < MAX_SERVER_ADDR ){
				strncpy( srv[*count].ip, ip, 31 );
				srv[*count].port = atoi( port );
//				printf("%s:%d\n", srv[*count].ip, srv[*count].port );
				(*count) ++;
			}
			if( *p=='\0' )
				break;
		}else{
			if( read_name ){
				if( j<31 )	ip[j++] = *p;
			}else{
				if( j<9 ) port[j++] = *p;
			}
		}
	}
}

static void read_config( qqclient* qq )
{
	assert( g_conf );
	if( !tcp_server_count && !udp_server_count ){
        //const char* tcps, *udps;
		char* tcps, *udps;      //---amoblin:把字符串从函数参数里调出，赋给数组再用。
		tcps = config_readstr( g_conf, "QQTcpServerList");
		udps = config_readstr( g_conf, "QQUdpServerList");
		if( tcps ){
			read_server_addr( tcp_servers, tcps, &tcp_server_count );
		}
		if( udps ){
			read_server_addr( udp_servers, udps, &udp_server_count );
		}
	}
	qq->log_packet = config_readint( g_conf, "QQPacketLog");
	if( config_readstr( g_conf, "QQNetwork") && stricmp( config_readstr( g_conf, "QQNetwork"), "TCP" ) == 0 )
		qq->network = TCP;
	else
		qq->network = UDP;
	if( config_readstr( g_conf, "QQVerifyDir" ) )
		strncpy( qq->verify_dir, config_readstr( g_conf, "QQVerifyDir" ), PATH_LEN );
	if( qq->verify_dir == NULL )
		strcpy( qq->verify_dir, "./web/verify" );
	mkdir_recursive( qq->verify_dir );
}

int qqclient_create( qqclient* qq, uint num, char* pass )
{
	uchar md5_pass[16];
	//加密密码
	md5_state_t mst;
	md5_init( &mst );
	md5_append( &mst, (md5_byte_t*)pass, strlen(pass) );
	md5_finish( &mst, (md5_byte_t*)md5_pass );
	return qqclient_md5_create( qq, num, md5_pass );
}


static void delete_func(const void *p)
{
	DEL( p );
}

int qqclient_md5_create( qqclient* qq, uint num, uchar* md5_pass )
{
	md5_state_t mst;
	//make sure all zero
	memset( qq, 0, sizeof( qqclient ) );
	qq->number = num;
	memcpy( qq->md5_pass1, md5_pass, 16 );
	//
	md5_init( &mst );
	md5_append( &mst, (md5_byte_t*)qq->md5_pass1, 16 );
	md5_finish( &mst, (md5_byte_t*)qq->md5_pass2 );
	qq->mode = QQ_ONLINE;
	qq->process = P_INIT;
	read_config( qq );
	qq->version = QQ_VERSION;
	//
	list_create( &qq->buddy_list, MAX_BUDDY );
	list_create( &qq->qun_list, MAX_QUN );
	list_create( &qq->group_list, MAX_GROUP );
	loop_create( &qq->event_loop, MAX_EVENT, delete_func );
	loop_create( &qq->msg_loop, MAX_EVENT, delete_func );
	pthread_mutex_init( &qq->mutex_event, NULL );
	//create self info
	qq->self = buddy_get( qq, qq->number, 1 );
	if( !qq->self ){
		DBG("[%d] Fatal error: qq->self == NULL", qq->number);
		return -1;
	}
	return 0;
}

#define INTERVAL 5
static void* qqclient_keepalive( void* data )
{
	qqclient* qq = (qqclient*) data;
	int counter = 0;
	DBG("keepalive");
	while( qq->process != P_INIT ){
		counter ++;
		if( counter % INTERVAL == 0 ){
			if( qq->process == P_LOGGING || qq->process == P_LOGIN ){
				packetmgr_check_packet( qq, 5 );
				if( qq->process == P_LOGIN ){
					//1次心跳/分钟
					if( counter % ( 1 *60*INTERVAL) == 0 ){
						prot_user_keep_alive( qq );
					}
					//10分钟刷新在线列表 QQ2009是5分钟刷新一次。
					if( counter % ( 10 *60*INTERVAL) == 0 ){
						prot_buddy_update_online( qq, 0 );
						qun_update_online_all( qq );
					}
					//30分钟刷新状态和刷新等级
					if( counter % ( 30 *60*INTERVAL) == 0 ){
						prot_user_change_status( qq );
						prot_user_get_level( qq );
					}
				//	if( qq->online_clock == 0 ){	//刚登录上了
				//		memcpy( last_server_info, qq->data.server_data, 15 );
				//		last_server_ip = qq->server_ip;
				//		last_server_port = qq->server_port;
				//	}
				//	//等待登录完毕
					if( ! qq->login_finish ){
						if( loop_is_empty(&qq->packetmgr.ready_loop) && 
							loop_is_empty(&qq->packetmgr.sent_loop) ){
							qq->login_finish = 1;	//we can recv message now.
						}
					}
					qq->online_clock ++;
				}
			}
		}
		USLEEP( 1000/INTERVAL );
	}
	DBG("end.");
	return NULL;
}


int connect_server( qqclient* qq )
{
	//connect server
	if( qq->socket )
		qqsocket_close( qq->socket );
	if( qq->network == TCP ){
		qq->socket = qqsocket_create( TCP, NULL, 0 );
	}else{
		qq->socket = qqsocket_create( UDP, NULL, 0 );
	}
	if( qq->socket < 0 ){
		DBG("can't not create socket. ret=%d", qq->socket );
		return -1;
	}
	struct in_addr addr;
	addr.s_addr = htonl( qq->server_ip );
	DBG("connecting to %s:%d", inet_ntoa( addr ), qq->server_port );
	if( qqsocket_connect2( qq->socket, qq->server_ip, qq->server_port ) < 0 ){
		DBG("can't not connect server %s", inet_ntoa( addr ) );
		return -1;
	}
	return 0;
}

void qqclient_get_server(qqclient* qq)
{
	int i;
	struct sockaddr_in addr;
	if( last_server_ip == 0 ){
		//random an ip
		if( qq->network == TCP && tcp_server_count>0 ){
			i = rand()%tcp_server_count;
			netaddr_set( tcp_servers[i].ip, &addr );
			qq->server_ip = ntohl( addr.sin_addr.s_addr );
			qq->server_port = tcp_servers[i].port;
		}else{
			if( udp_server_count <1 ){
				qq->process = P_ERROR;
				DBG("no server for logging.");
			}
			i = rand()%udp_server_count;
			netaddr_set( udp_servers[i].ip, &addr );
			qq->server_ip = ntohl( addr.sin_addr.s_addr );
			qq->server_port = udp_servers[i].port;
		}
	}else{
		qq->server_ip = last_server_ip;
		qq->server_port = last_server_port;
		last_server_ip = 0;
//		memcpy( qq->data.server_data, last_server_info, 15 );
	}
}

int qqclient_login( qqclient* qq )
{
	DBG("login");
	int ret;
	if( qq->process != P_INIT ){
		DBG("please logout first");
		return -1;
	}
	qqclient_set_process( qq, P_LOGGING );
	srand( qq->number + time(NULL) );
	//start packetmgr
	packetmgr_start( qq );
	packetmgr_new_seqno( qq );
	qqclient_get_server( qq );
	ret = connect_server( qq );
	if( ret < 0 ){
		qq->process = P_ERROR;
		return ret;
	}
	//ok, already connected to the server
	//send touch packet
//	if( last_server_ip == 0 ){
		prot_login_touch( qq );
//	}else{
//		prot_login_touch_with_info( qq, last_server_info, 15 );
//	}
	//keep
	ret = pthread_create( &qq->thread_keepalive, NULL, qqclient_keepalive, (void*)qq );
	return 0;
}

void qqclient_logout( qqclient* qq )
{
	if( qq->process == P_INIT )
		return;
	if( qq->process == P_LOGIN ){
		int i;
		for( i = 0; i<4; i++ )
			prot_login_logout( qq );
	}else{
		DBG("process = %d", qq->process );
	}
	qq->login_finish = 0;
	qqclient_set_process( qq, P_INIT );
	qqsocket_close( qq->http_sock );
	qqsocket_close( qq->socket );
	DBG("joining keepalive");
#ifdef __WIN32__
	pthread_join( qq->thread_keepalive, NULL );
#else
	if( qq->thread_keepalive )
		pthread_join( qq->thread_keepalive, NULL );
#endif
	packetmgr_end( qq );
}


void qqclient_cleanup( qqclient* qq )
{
	if( qq->process != P_INIT )
		qqclient_logout( qq );
	pthread_mutex_lock( &qq->mutex_event );
	qun_member_cleanup( qq );
	list_cleanup( &qq->buddy_list );
	list_cleanup( &qq->qun_list );
	list_cleanup( &qq->group_list );
	loop_cleanup( &qq->event_loop );
	loop_cleanup( &qq->msg_loop );
	pthread_mutex_destroy( &qq->mutex_event );
}

int qqclient_verify( qqclient* qq, uint code )
{
	if( qq->login_finish ){
		prot_user_request_token( qq, qq->data.operating_number, qq->data.operation, 1, code );
	}else{
		qqclient_set_process( qq, P_LOGGING );
		prot_login_request( qq, &qq->data.verify_token, code, 0 );
	}
	DBG("verify code: %x", code );
	return 0;
}

int qqclient_add( qqclient* qq, uint number, char* request_str )
{
	qqbuddy* b = buddy_get( qq, number, 0 );
	if( b && b->verify_flag == VF_OK )	{
		prot_buddy_verify_addbuddy( qq, 03, number );	//允许一个请求x
	}else{
		strncpy( qq->data.addbuddy_str, request_str, 50 );
		prot_buddy_request_addbuddy( qq, number );
	}
	return 0;
}

int qqclient_del( qqclient* qq, uint number )
{
	qq->data.operating_number = number;
	prot_user_request_token( qq, number, OP_DELBUDDY, 6, 0 );
	return 0;
}

int qqclient_wait( qqclient* qq, int sec )
{
	int i;
	//we don't want to cleanup the data while another thread is waiting here.
	if( pthread_mutex_trylock( &qq->mutex_event ) != 0 )
		return -1;	//busy?
	for( i=0; (sec==0 || i<sec) && qq->process!=P_INIT; i++ ){
		if( loop_is_empty(&qq->packetmgr.ready_loop) && loop_is_empty(&qq->packetmgr.sent_loop) )
		{
			pthread_mutex_unlock( &qq->mutex_event );
			return 0;
		}
		SLEEP(1);
	}
	pthread_mutex_unlock( &qq->mutex_event );
	return -1;
}

void qqclient_change_status( qqclient* qq, uchar mode )
{
	qq->mode = mode;
	prot_user_change_status( qq );
}

// wait: <0   wait until event arrives
// wait: 0  don't need to wait, return directly if no event
// wait: n(n>0) wait n secondes.
// return: 1:ok  0:no event
int qqclient_get_event( qqclient* qq, char* event, int size, int wait )
{
	char* buf;
	//we don't want to cleanup the data while another thread is waiting here.
	if( pthread_mutex_trylock( &qq->mutex_event ) != 0 )
		return -1;	//busy?
	for( ; ; ){
		if(  qq->process == P_INIT ){
			pthread_mutex_unlock( &qq->mutex_event );
			return -1;
		}
		buf =(char *) loop_pop_from_head( &qq->event_loop );
		if( buf ){
			int len = strlen( buf );
			if( len < size ){
				strcpy( event, buf );
			}else{
				DBG("buffer too small.");
			}
			delete_func( buf );
			pthread_mutex_unlock( &qq->mutex_event );
			return 1;
		}
		if( qq->online_clock > 10 ){
			buf =(char *) loop_pop_from_head( &qq->msg_loop );
			if( buf ){
				int len = strlen( buf );
				if( len < size ){
					strcpy( event, buf );
				}else{
					DBG("buffer too small.");
				}
				delete_func( buf );
				pthread_mutex_unlock( &qq->mutex_event );
				return 1;
			}
		}
		if( wait<0 || wait> 0 ){
			if( wait>0) wait--;
			USLEEP( 200 );
		}else{
			break;
		}
	}
	pthread_mutex_unlock( &qq->mutex_event );
	return 0;
}

int qqclient_put_event( qqclient* qq, char* event )
{
	char* buf;
	int len = strlen( event );

	//NEW( buf, len+1 );
    //amoblin:redefine NEW
    char buf_tmp[len+1];
    memset( buf_tmp, 0, len+1);
    buf = buf_tmp;

	if( !buf ) return -1;
	strcpy( buf, event );
	loop_push_to_tail( &qq->event_loop, (void*)buf );
	return 0;
}

int qqclient_put_message( qqclient* qq, char* msg )
{
	char* buf;
	int len = strlen( msg );
	//NEW( buf, len+1 );

    char buf_tmp[len+1];
    memset( buf_tmp, 0, len+1);
    buf = buf_tmp;
	if( !buf ) return -1;

	strcpy( buf, msg );
	loop_push_to_tail( &qq->msg_loop, (void*)buf );
	return 0;
}



void qqclient_set_process( qqclient *qq, int process )
{
	qq->process = process;
	char event[16];
	sprintf( event, "process^$%d", process );
	qqclient_put_event( qq, event );
}
