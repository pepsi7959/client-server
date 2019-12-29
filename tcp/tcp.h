#ifndef __NPP_UDP_H
#define __NPP_UDP_H

#define DEFAULT_TCP_PORT 8081
#define DEFAULT_UDP_PORT 8081
#define MAX_BUFFER 1000

struct udef_msg{
	int id;
	size_t len;
	size_t size;
	char data[1];
};


typedef struct udef_msg UMSG; 

#endif
