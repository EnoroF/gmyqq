/*
 *  prot_login.c
 *
 *  QQ Protocol. Part Login
 *
 *  Copyright (C) 2008  Huang Guan
 *
 *  2008-7-12 Created.
 *
 *  Description: This file mainly includes the functions about 
 *  Touching server, getting login token, getting password token, getting loginkey.
 *  sending login information, logging out.
 *
 */

#include <time.h>
#include <stdlib.h>
#include <string.h>
#ifdef __WIN32__
#include <winsock.h>
#include <wininet.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include "qqclient.h"
#include "memory.h"
#include "debug.h"
#include "qqpacket.h"
#include "packetmgr.h"
#include "qqcrypt.h"
#include "md5.h"
#include "crc32.h"
#include "protocol.h"
#include "group.h"
#include "buddy.h"
#include "qun.h"

#define RANDOM
#ifdef RANDOM
#endif
static int rand2()
{
	return 0;//(rand()<<16)+rand();
}
static void randkey(uchar* key)
{
	int i;
	for( i=0; i<16; i++ )
		key[i] = rand2();
}

static void restore_version_data( struct qqclient* qq )
{
	static uchar QQ09_LOCALE[] = {0x00,0x00,0x08,0x04,0x01,0xE0}; 
	static uchar QQ09_VERSION_SPEC[] = {0x00,0x00,0x02,0x00,0x00,
		0x00,0x00,0x01,0x00,0x00,0x08,0x59}; 
	static uchar QQ09_EXE_HASH[] = {0x62,0x0C,0x12,0x15,0x53,0x7E,
		0x9F,0x12,0xA9,0x46,0xBC,0x54,0x41,0xC1,0x09,0x53};
	memcpy( qq->data.locale, QQ09_LOCALE, sizeof(QQ09_LOCALE) );
	memcpy( qq->data.version_spec, QQ09_VERSION_SPEC, 
		sizeof(QQ09_VERSION_SPEC) );
	memcpy( qq->data.exe_hash, QQ09_EXE_HASH, 
		sizeof(QQ09_EXE_HASH) );
}

void prot_login_touch( struct qqclient* qq )
{
	static uchar zeros[15]={0,};
	prot_login_touch_with_info( qq, zeros, sizeof(zeros) );
}

void prot_login_touch_with_info( struct qqclient* qq, uchar* server_data, uchar len )
{
	qqpacket* p = packetmgr_new_send( qq, QQ_CMD_TOUCH );
	if( !p ) return;
	bytebuffer *buf = p->buf;
	restore_version_data( qq );
	randkey( p->key );
	memcpy( qq->data.server_data, server_data, MIN_(len,sizeof(qq->data.server_data)) );
	put_word( buf, 1 );
	put_data( buf, qq->data.locale, sizeof(qq->data.locale) );
	put_data( buf, qq->data.version_spec, sizeof(qq->data.version_spec) );
	put_data( buf, server_data, sizeof(qq->data.server_data) );
	post_packet( qq, p, RANDOM_KEY );
}

void prot_login_touch_reply( struct qqclient* qq, qqpacket* p )
{
	bytebuffer *buf = p->buf;
	uchar result = get_byte( buf ), test_result;
	if( result == 0x00 ){
		qq->server_time = get_int( buf );
		qq->server_ip = get_int( buf );
		buf->pos += 8;	//zeros
		get_token( buf, &qq->data.login_token );
		test_result = get_byte( buf );
		if( test_result == 0 ){
			prot_login_request( qq, NULL, 0, 0 );
		}else{
			qq->data.server_info.w_redirect_count = 1;
			qq->data.server_info.c_redirect_count = get_byte( buf );
			qq->data.server_info.conn_isp_id = get_int( buf );
			qq->data.server_info.server_reverse = get_int( buf );
			qq->data.server_info.conn_ip = qq->server_ip;
			qq->server_ip = get_int( buf );
			struct in_addr addr;
			addr.s_addr = htonl( qq->server_ip );
			DBG("redirecting to %s", inet_ntoa( addr ) );
			connect_server( qq );
			prot_login_touch_with_info( qq, qq->data.server_data, sizeof(qq->data.server_data) );
		}
	}else{
		DBG("result!=00: %d", result );
	}
}

void prot_login_request( struct qqclient* qq, token* tok, uint code, char png_data )
{
	
	qqpacket* p = packetmgr_new_send( qq, QQ_CMD_LOGIN_REQUEST );
	if( !p ) return;
	bytebuffer *buf = p->buf;
	put_word( buf, 0x0001 );
	put_data( buf, qq->data.locale, sizeof(qq->data.locale) );
	put_data( buf, qq->data.version_spec, sizeof(qq->data.version_spec) );
	//
	put_word( buf, qq->data.login_token.len );
	put_data( buf, qq->data.login_token.data, qq->data.login_token.len );
	if( code )
		put_byte( buf, 4 );
	else
		put_byte( buf, 3 );
	put_byte( buf, 0 );
	put_byte( buf, 5 );
	put_int( buf, 0 );
	put_byte( buf, png_data );
	if( code && tok ){
		put_byte( buf, 4 );
		put_int( buf, htonl( code ) );
		//answer token
		put_word( buf, tok->len );
		put_data( buf, tok->data, tok->len );
	}else if( png_data && tok ){
		//png token
		put_word( buf, tok->len );
		put_data( buf, tok->data, tok->len );
	}else{
		put_byte( buf, 0 );
		put_byte( buf, 0 );
	}
	post_packet( qq, p, RANDOM_KEY );
}

void prot_login_request_reply( struct qqclient* qq, qqpacket* p )
{
	bytebuffer *buf = p->buf;
	token answer_token;
	token png_token;
	uchar next = 0;
	uchar result = get_byte( buf );	//03: ok   04: need verifying
	get_byte( buf ); //00
	get_byte( buf ); //05
	uchar png_data = get_byte( buf );
	get_int( buf );	//需要验证码时为00 00 01 23，不需要时为全0
	get_token( buf, &answer_token );
	if( png_data == 1 ){	//there's data for the png picture
		int len;
		len = get_word( buf );
		uchar* data;
		NEW( data, len ,uchar);
		if( !data )
			return;
		get_data( buf, data, len );
		get_byte( buf ); //00
		next = get_byte( buf );
		char path[PATH_LEN];
		sprintf( path, "%s/%u.png", qq->verify_dir, qq->number );
		FILE *fp;
		if( next ){
			fp = fopen( path, "wb" );
		}else{
			fp = fopen( path, "ab" );
			DBG("got png at %s", path );
		}
		if( fp ){
			fwrite( data, len, 1, fp );
			fclose( fp );
		}
		DEL( data );
		get_token( buf, &png_token );
	}
	//parse the data we got
	if( png_data ){
		if( next ){
			prot_login_request( qq, &png_token, 0, 1 );
		}else{
			qq->data.verify_token = answer_token;
			qqclient_set_process( qq, P_VERIFYING );
		}
	}else{
		DBG("process verify password");
		qq->data.token_c = answer_token;
		prot_login_verify( qq );
	}
	result = 0;
}


void prot_login_verify( struct qqclient* qq )
{
	qqpacket* p = packetmgr_new_send( qq, QQ_CMD_LOGIN_VERIFY );
	if( !p ) return;
	bytebuffer *buf = p->buf;
	bytebuffer *verify_data;
	NEW( verify_data, sizeof(bytebuffer) ,bytebuffer);
	if( !verify_data ) {
		packetmgr_del_packet( &qq->packetmgr, p );
		return;
	}
	verify_data->size = PACKET_SIZE;
	put_int( verify_data, 0xEE21B81B );	//random??
	put_word( verify_data, 0x0001 );
	put_int( verify_data, qq->number );
	put_data( verify_data, qq->data.version_spec, sizeof(qq->data.version_spec) );
	put_byte( verify_data, 00 );
	put_word( verify_data, 0x0001 );
	put_data( verify_data, qq->md5_pass1, sizeof(qq->md5_pass1) );
	put_int( verify_data, qq->server_time );
	verify_data->pos += 13;
	put_int( verify_data, qq->client_ip );
	put_int( verify_data, 0 );
	put_int( verify_data, 0 );
	put_word( verify_data, 0x0010 );
	put_data( verify_data, qq->data.verify_key1, 0x10 );
	put_data( verify_data, qq->data.verify_key2, 0x10 );
	//
	put_word( buf, 0x00CA );	//sub cmd??
	put_word( buf, 0x0001 );
	put_data( buf, qq->data.locale, sizeof(qq->data.locale) );
	put_data( buf, qq->data.version_spec, sizeof(qq->data.version_spec) );
	put_word( buf, qq->data.token_c.len );
	put_data( buf, qq->data.token_c.data, qq->data.token_c.len );
	if( verify_data->pos != 104 ){ DBG("wrong pos!!!");	}
	
	int out_len = 120;
	uchar encrypted[120+10];
	qqencrypt( verify_data->data, verify_data->pos, qq->md5_pass2, encrypted, &out_len );
	put_word( buf, out_len );
	put_data( buf, encrypted, out_len );
	
	put_word( buf, 0x0000 );
	put_word( buf, 0x018B );
	buf->pos += 0x018B;	//395 zeros?
	
	DEL( verify_data );
	post_packet( qq, p, RANDOM_KEY );
}

void prot_login_verify_reply( struct qqclient* qq, qqpacket* p )
{
	bytebuffer *buf = p->buf;
	get_word( buf );	//00 F9  or  00 94(激活)  or 00 c6(密码错误)
	int code = get_byte( buf );
	switch( code ){
	case 0x00:
		get_token( buf, &qq->data.login_info_token );
		qq->data.login_info_unknown1 = get_int( buf );
		qq->server_time = get_int( buf );
		get_token( buf, &qq->data.login_info_data );
		get_token( buf, &qq->data.login_info_magic );
		get_data( buf, qq->data.login_info_key1, 16 );
		//success
		prot_login_get_info( qq );
		return;
	case 0x33:
	case 0x51:	//
		qqclient_set_process( qq, P_DENIED );
		DBG("Denied.");
		break;
	case 0xBF:
		DBG("No this number.");
	case 0x34:
		DBG("Wrong password.");
		qqclient_set_process( qq, P_WRONGPASS );
		break;
	default:
		hex_dump( buf->data, buf->len );
		break;
	}
	buf->pos = 11;
	uchar len = get_word( buf );
	char msg[256];
	get_data( buf, (uchar*)msg, len );
	msg[len] = 0;
	DBG( msg );
	//failed
}


void prot_login_get_info( struct qqclient* qq )
{
	qqpacket* p = packetmgr_new_send( qq, QQ_CMD_LOGIN_GET_INFO );
	if( !p ) return;
	bytebuffer *buf = p->buf;
	put_word( buf, 0x00FD );
	put_byte( buf, 0x00 );
	put_word( buf, 0x0101 );
	put_data( buf, qq->data.locale, sizeof(qq->data.locale) );
	put_data( buf, qq->data.version_spec, sizeof(qq->data.version_spec) );
	put_word( buf, qq->data.token_c.len );
	put_data( buf, qq->data.token_c.data, qq->data.token_c.len );
	put_word( buf, qq->data.login_info_token.len );
	put_data( buf, qq->data.login_info_token.data, qq->data.login_info_token.len );
	put_int( buf, qq->data.login_info_unknown1 );
	put_int( buf, qq->server_time );
	put_word( buf, qq->data.login_info_data.len );
	put_data( buf, qq->data.login_info_data.data, qq->data.login_info_data.len );
	put_word( buf, 0x0000 );
	put_int( buf, 0x00000000 );
	memcpy( p->key, qq->data.login_info_key1, sizeof(qq->data.login_info_key1) );
	post_packet( qq, p, RANDOM_KEY );
}

void prot_login_get_info_reply( struct qqclient* qq, qqpacket* p )
{
	bytebuffer *buf = p->buf;
	get_word( buf );	//00 E5
	get_word( buf );	//01 00
	get_data( buf, qq->data.login_info_key2, 16 );
	buf->pos += 8;	//00 00 00 01 00 00 00 64 00 B8 00 01
	qq->data.login_info_unknown2 = get_int( buf );
	qq->server_time = get_int( buf );
	qq->client_ip = get_int( buf );
	get_int( buf );	//00000000
	get_token( buf, &qq->data.login_info_large );
	get_int( buf );	//????
	uchar len = get_byte( buf );
	get_data( buf, (uchar*)qq->self->nickname, len );
	qq->self->nickname[len] = 0;
	DBG("Hello, %s", qq->self->nickname );
	//2009-1-26 19:04 Huang Guan: for getting the whole list
	qq->data.login_list_count = 0;
	prot_login_get_list( qq, 0 );
}


void prot_login_get_list( struct qqclient* qq, ushort pos )
{
	qqpacket* p = packetmgr_new_send( qq, QQ_CMD_LOGIN_GET_LIST );
	if( !p ) return;
	bytebuffer *buf = p->buf;
	put_word( buf, 0x010A );
	put_word( buf, 0x0001 );
	put_data( buf, qq->data.locale, sizeof(qq->data.locale) );
	put_data( buf, qq->data.version_spec, sizeof(qq->data.version_spec) );
	put_word( buf, qq->data.token_c.len );
	put_data( buf, qq->data.token_c.data, qq->data.token_c.len );
	put_int( buf, qq->data.login_info_unknown2 );
	put_int( buf, qq->server_time );
	put_int( buf, qq->client_ip );
	put_int( buf, 00000000 );
	put_word( buf, qq->data.login_info_large.len );
	put_data( buf, qq->data.login_info_large.data, qq->data.login_info_large.len );
	put_word( buf, pos );
	put_word( buf, 0x0000 );
	put_word( buf, 0x0081 );
	buf->pos += 0x0081;	//0x0081 zeros
	memcpy( p->key, qq->data.login_info_key1, sizeof(qq->data.login_info_key1) );
	post_packet( qq, p, RANDOM_KEY );
}

void prot_login_get_list_reply( struct qqclient* qq, qqpacket* p )
{
	bytebuffer *buf = p->buf;
	ushort next_pos, len;
	len = get_word( buf );	//00 9C
	get_int( buf );	//00 00 00 00
	next_pos = get_word( buf );
	if( next_pos > 0x0000 ){
		//Well, i don't know how to judge whether we have got the whole list, so I just 
		//tested the length.
		DBG("next_pos = %d", next_pos );
		if( len == 0x038A ){
			prot_login_get_list( qq, ++ qq->data.login_list_count );
		}
	}else{
		prot_login_send_info( qq );
	}
	while( buf->pos < buf->len-2 ){	//2zeros in the end
		uint number = get_int( buf );
		uchar type = get_byte( buf );
		uchar gid = get_byte( buf );
		if( type == 0x04 )	//if it is a qun
		{
#ifndef NO_QUN_INFO
			qun_get( qq, number, 1 );
#endif
		}else if( type == 0x01 ){
#ifndef NO_BUDDY_INFO
			qqbuddy* b = buddy_get( qq, number, 1 );
//			DBG("QQ: %d", number );
			b->gid = gid / 4;
#endif
		}
	}
}


void prot_login_send_info( struct qqclient* qq )
{
	static uchar unknown5[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
		0x00, 0x00, 0x00 };
	static uchar unknown6[] = {0x3C,0xE7,0x06,0xE8,0x28,0xE9,
		0xCA,0x19,0x3B,0x87,0xAE,0xB7,0x40,0x43,0x9F,0xA9 };
	static uchar unknown7[] = {0x34,0x40,0xA8,0x12,0x97,0xE3,
		0x0B,0x3D,0xF6,0x8B,0x18,0xB6,0xC9,0x63,0xC0,0x88 };
		
	qqpacket* p = packetmgr_new_send( qq, QQ_CMD_LOGIN_SEND_INFO );
	if( !p ) return;
	bytebuffer *buf = p->buf;
	//prepare sth.
	randkey( unknown6 );
	randkey( unknown7 );
	
	put_word( buf, 0x0001 );
	put_data( buf, qq->data.version_spec, sizeof(qq->data.version_spec) );
	put_int( buf, qq->data.login_info_unknown2 );
	put_int( buf, qq->server_time );
	put_int( buf, qq->client_ip );
	put_int( buf, 00000000 );
	put_word( buf, qq->data.login_info_large.len );
	put_data( buf, qq->data.login_info_large.data, qq->data.login_info_large.len );
	buf->pos += 35;
	put_data( buf, qq->data.exe_hash, sizeof(qq->data.exe_hash) );
	put_byte( buf, rand() );	//unknown important byte
	put_byte( buf, qq->mode );
	put_data( buf, unknown5, sizeof(unknown5) );
	put_data( buf, qq->data.server_data, sizeof(qq->data.server_data) );
	put_data( buf, qq->data.locale, sizeof(qq->data.locale) );
	buf->pos += 16;
	put_word( buf, qq->data.token_c.len );
	put_data( buf, qq->data.token_c.data, qq->data.token_c.len );
	put_int( buf, 0x00000007 );
	put_int( buf, 0x00000000 );
	put_word( buf, 0x0804 );
	put_int( buf, 0x10014001 );
	put_word( buf, sizeof(unknown6) );
	put_data( buf, unknown6, sizeof(unknown6) );
	put_data( buf, unknown5, sizeof(unknown5) );
	put_data( buf, qq->data.server_data, sizeof(qq->data.server_data) );
	put_byte( buf, 0x02 );
	put_int( buf, 0x812807B8 );
	put_word( buf, sizeof(unknown7) );
	put_data( buf, unknown7, sizeof(unknown7) );
	buf->pos += 249;	//all zeros
	memcpy( p->key, qq->data.login_info_key1, sizeof(qq->data.login_info_key1) );
	post_packet( qq, p, RANDOM_KEY );
}

void prot_login_send_info_reply( struct qqclient* qq, qqpacket* p )
{
	bytebuffer *buf = p->buf;
//	hex_dump( buf->data, buf->len );
	uchar result = get_byte( buf );
	if( result != 0 )
	{
		DBG("login result = %d", result );
		qqclient_set_process( qq, P_ERROR );
		return;
	}
	get_data( buf, qq->data.session_key, sizeof(qq->data.session_key) );
	DBG("session key: " );
	hex_dump( qq->data.session_key, 16 );
	if( qq->number != get_int( buf ) ){
		DBG("qq->number is wrong?");
	}
	qq->client_ip = get_int( buf );
	qq->client_port = get_word( buf );
	qq->local_ip = get_int( buf );
	qq->local_port = get_word( buf );
	qq->login_time = get_int( buf );
	get_byte( buf );	//03
	get_byte( buf );	//mode
	buf->pos += 96;
	qq->last_login_time = get_int( buf );
	//prepare IM key
	uchar data[20];
	*(uint*)data = htonl( qq->number );
	memcpy( data+4, qq->data.session_key, 16 );
	//md5
	md5_state_t mst;
	md5_init( &mst );
	md5_append( &mst, (md5_byte_t*)data, 20 );
	md5_finish( &mst, (md5_byte_t*)qq->data.im_key );
	//
	time_t t;
	t = CN_TIME( qq->last_login_time );
	DBG("last login time: %s", ctime( &t ) );
	qqclient_set_process( qq, P_LOGIN );

	//get information
	prot_user_change_status( qq );
	prot_user_get_level( qq );
	group_update_list( qq );
	buddy_update_list( qq );
#ifndef NO_QUN_INFO
	qun_update_all( qq );
#endif
	qq->online_clock = 0;
}


void prot_login_logout( struct qqclient* qq )
{
	qqpacket* p = packetmgr_new_send( qq, QQ_CMD_LOGOUT );
	if( !p ) return;
	bytebuffer *buf = p->buf;
	buf->pos += 16;	//zeros
	p->need_ack = 0;
	post_packet( qq, p, SESSION_KEY );
}
