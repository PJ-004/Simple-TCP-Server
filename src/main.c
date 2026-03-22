#include "server.h"

int main() {
	/* Create Socket */
	int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock < 0) {
		perror("Error: Unable to create socket");
		return 1;
	}

	/* Set socket to be reuseable */
	int opt = 1;
	if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt failed");
	}

	/* Fill the sockaddr_in struct */
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(LOCAL_PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	socklen_t len = sizeof(serv_addr);

	server_loop(serv_sock, (struct sockaddr *)&serv_addr, len);

	return 0;
}
