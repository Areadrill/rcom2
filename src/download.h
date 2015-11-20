#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#define FTP_PORT 21
struct addrinfo* getIP(char* name);

char urlContent[256];

typedef struct URL
{
	urlContent user; //string user
	urlContent password; //string password
	urlContent hostname; //string host
	urlContent ip; //string IP
	urlContent filename; //string filename
	int port;
};


void initURL(url* url);
