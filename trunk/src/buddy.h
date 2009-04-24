#ifndef _BUDDY_H
#define _BUDDY_H

#include "qqdef.h"

typedef struct qqbuddy{
	uint 		number;
	char 		nickname[NICKNAME_LEN];
	uint 		ip;
	ushort 		port;
	ushort		face;
	uchar		age;
	uchar		sex;
	uchar		gid;
	uchar		qqshow;
	uchar		flag;
	uchar		session_key[16];
	uchar		status;
	ushort		version;
	uchar		verify_flag;	//00 允许 01 验证 02 拒绝 03 问题
	
	uint		sign_time;
	uchar		account_flag;
#ifndef NO_BUDDY_DETAIL_INFO
	char		signiture[SIGNITURE_LEN];
	char		account[ACCOUNT_LEN];	//email account
	char		alias[ALIAS_LEN];
	char		info_string[MAX_USER_INFO][USER_INFO_LEN];
#else
	char		signiture[1];
	char		account[1];
	char		alias[1];
	char		info_string[1][1];
#endif

}qqbuddy;

struct qqclient;
qqbuddy* buddy_get( struct qqclient* qq, uint number, int create );
void buddy_remove( struct qqclient* qq, uint number );
void buddy_update_list( struct qqclient* qq );
void buddy_update_info( struct qqclient* qq, qqbuddy* b );
void buddy_sort_list( struct qqclient* qq );
int buddy_send_message( struct qqclient* qq, uint number, char* msg );
void buddy_set_all_off( struct qqclient* qq );
void buddy_put_event( struct qqclient* qq );
char* buddy_status_string( int st );

#endif
