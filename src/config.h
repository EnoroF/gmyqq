//config.h
#ifndef _CONFIG_H
#define _CONFIG_H

#define CONFIG_NAME_LEN 64
#define CONFIG_VALUE_LEN 256


typedef struct config_item{
	char name[CONFIG_NAME_LEN];
	char value[CONFIG_VALUE_LEN];
}config_item;

typedef struct cconfig{
	int item_count;
	config_item *items[1024];
}config;

//amobline:add const to argument 2
//int config_open( config* c, const char* filename );
//int config_readint( config*c, const char* name );
//char* config_readstr( config*c, const char* name );
//void config_close( config* c );

//
void config_init();
void config_end();
extern config *g_conf;

#endif
