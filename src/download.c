#include "download.h"

struct addrinfo* getIP(char* name){
	struct addrinfo hints;
	memset(&hints, 0, sizeof hints);
	//we are only interested in IPv4 address of sock_stream socket type
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *info;
	memset(&info, 0, sizeof hints);
	char port[3];
	sprintf(port, "%d", FTP_PORT);
	int r = getaddrinfo(name,port, &hints, &info);

	if(r != 0){
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(r));
		return NULL;
	}
	
	struct sockaddr_in *ip = (struct sockaddr_in*)info->ai_addr; 
	struct in_addr *addr = &(ip->sin_addr);
	char* straddr = malloc(100);
	inet_ntop(AF_INET, addr, straddr, 100);
	fprintf(stdout, "Address translated to ip %s\n", straddr);
	free(straddr);
	return info;
}

void initURL(url* url){
	memset(url->user, 0, sizeof(urlContent));
	memset(url->password, 0, sizeof(urlContent));
	memset(url->host, 0, sizeof(urlContent));
	memset(url->path, 0, sizeof(urlContent));
	memset(url->filename, 0, sizeof(urlContent));
	url->port = FTP_PORT;
}

int main(int argc, char** argv){
	if (argc != 2){
		puts("incorrect usage of program");
		exit(-1);
	}
	struct addrinfo *address = getIP(argv[1]);
	int socketfd = socket(address->ai_family, address->ai_socktype, address->ai_protocol);	
	if(socketfd == -1){
		perror("couldn't open socket, aborting:");
		exit(-1);
	}
	int connectstatus = connect(socketfd, address->ai_addr, address->ai_addrlen);
	if(connectstatus == -1){
		perror("couldn't connect to server, aborting:");
		exit(-1);
	}
	else
		puts("connection successfully made");
}


