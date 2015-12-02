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



int main(int argc, char** argv){
	if (argc != 2){
		printf("Usage: %s ftp://[username:password]@site/path/to/file", argv[0]);
		exit(-1);
	}
	
	URLData *data = parseURL(argv[1]);		
	
	
	struct addrinfo *address = getIP(data->hostname);
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

	char *userMsg = (char *) malloc(8+strlen(data->user));
	sprintf(userMsg, "USER %s\r\n", data->user);
	char *passMsg = (char *) malloc(8+strlen(data->password));
	sprintf(passMsg, "PASS %s\r\n", data->password);
	puts(userMsg);
	char *responses = malloc(1000);
	int i=0;

	recv(socketfd, responses, 1000, 0);
	printf("%d - %s", i++, responses);
	int sent = write(socketfd, userMsg, strlen(userMsg));	
	if(sent <= 0)
		puts("si fodeu");
	recv(socketfd, responses, 1000, 0);
	printf("%d - %s", i++, responses);
	memset(responses, 1000, 0);
	puts(passMsg);
	sent = send(socketfd, passMsg, strlen(passMsg), 0);
	recv(socketfd, responses, 1000, 0);
	if(sent <= 0)
		puts("si fodeu");
	printf("%d - %s", i++, responses);
	
	
}

URLData* parseURL(const char* url){
	char* regexp = "ftp://(([^:]*):([^@]*)@)?([^/]+)/(.*/)?([^/]*)"; 
	regex_t regex;
	int r =	regcomp(&regex, regexp, REG_EXTENDED);
	if (r != 0){
		perror("regex not compiled:");
		return NULL;
	}
	regmatch_t pmatch[7];
	r = regexec(&regex, url, 7, pmatch, 0);
	if (r == REG_NOMATCH){
		perror("regex not matches:");
		return NULL;
	}
	//grupo 0 1 6 pra descartar	
	URLData* data = malloc(sizeof(URLData));
	data->user = malloc(1+pmatch[2].rm_eo-pmatch[2].rm_so);
	data->password = malloc(1+pmatch[3].rm_eo-pmatch[3].rm_so);
	data->hostname = malloc(1+pmatch[4].rm_eo-pmatch[4].rm_so);
	data->filepath = malloc(1+pmatch[5].rm_eo-pmatch[5].rm_so);
	data->filename = malloc(1+pmatch[6].rm_eo-pmatch[6].rm_so);
	strncpy(data->user, url+pmatch[2].rm_so, pmatch[2].rm_eo-pmatch[2].rm_so);
	strncpy(data->password, url+pmatch[3].rm_so, pmatch[3].rm_eo-pmatch[3].rm_so);
	strncpy(data->hostname, url+pmatch[4].rm_so, pmatch[4].rm_eo-pmatch[4].rm_so);
	strncpy(data->filepath, url+pmatch[5].rm_so, pmatch[5].rm_eo-pmatch[5].rm_so);
	strncpy(data->filename, url+pmatch[6].rm_so, pmatch[6].rm_eo-pmatch[6].rm_so);
	data->user[pmatch[2].rm_eo-pmatch[2].rm_so] = 0;
	data->password[pmatch[3].rm_eo-pmatch[3].rm_so] = 0;
	data->hostname[pmatch[4].rm_eo-pmatch[4].rm_so] = 0;
	data->filepath[pmatch[5].rm_eo-pmatch[5].rm_so] = 0;
	data->filename[pmatch[6].rm_eo-pmatch[6].rm_so] = 0;

	regfree(&regex);
	return data;
	
}



