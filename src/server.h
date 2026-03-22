#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LOCAL_PORT 	5000
#define LOCAL_IP 	"127.0.0.1"

void server_loop(int serv_sock, struct sockaddr* serv_addr, socklen_t servlen);
const char *get_content_type(const char *path);
size_t get_file_size(FILE *fp);

/* HTTP 200 OK header */
static const char *HTTP_200_Header = "HTTP/1.1 200 OK\r\n"
	"Content-Type: %s\r\n"
	"Content-Length: %zu\r\n"
	"Connection: close\r\n"
	"\r\n";

/* HTTP 404 Not Found response */
static const char *HTTP_404_Response = "HTTP/1.1 404 Not Found\r\n"
	"Content-Type: text/plain\r\n"
	"Content-Length: 9\r\n"
	"\r\n"
	"Not Found";

#endif
