#include "server.h"

void server_loop(int serv_sock, struct sockaddr* serv_addr, socklen_t servlen) {
	/* Bind on the local IP */
	int bind_err = bind(serv_sock, serv_addr, servlen);
	if (bind_err < 0) {
		perror("Error: bind() failed");
		return;
	}

	/* Make a listening socket */
	int listen_err = listen(serv_sock, 10);
	if (listen_err < 0) {
		perror("Error: listen() failed");
		return;
	}

	/* Main loop */
	while (1) {
		/* New sockaddr struct for storing client address */
		struct sockaddr_in cli_addr;
        	socklen_t cli_len = sizeof(cli_addr);
		
		/* Accept a client connection*/
		int cli_sock = accept(serv_sock, (struct sockaddr *)&cli_addr, &cli_len);
		if (cli_sock < 0) {
			perror("Error: Failed to create socket");
			return;
		}

		/* Read what nginx sent */
		char nginx_request[1024];
		memset(nginx_request, 0, sizeof(nginx_request));
		read(cli_sock, nginx_request, 1023);
		
		/* Parse HTTP request */
		char HTTP_method[10], filepath[256];
		if (sscanf(nginx_request, "%9s %255s", HTTP_method, filepath) < 2) {
			close(cli_sock);
			continue;
		}
		if (strcmp(HTTP_method, "GET") != 0) {
			fprintf(stderr, "Methods besides GET not supported");
			close(cli_sock);
			continue;
		}

		/* Get filename */
		char *filename = filepath + 1; // Remove leading slash
		if (strlen(filename) == 0) {
			filename = "index.html"; // Default to index.html
		}

		/* Read from file requested by client */
		FILE *fp = fopen(filename, "rb");
		if (!fp) {
			write(cli_sock, HTTP_404_Response, strlen(HTTP_404_Response));
			close(cli_sock);
			continue;
		}
		size_t file_len = get_file_size(fp);
		char *file_buffer = malloc(file_len);
		fread(file_buffer, 1, file_len, fp);
		fclose(fp);

		/* Make the header */
		char header[512];
		snprintf(header, sizeof(header), HTTP_200_Header, get_content_type(filename), file_len);

		/* Write HTTP Header to socket */
		int header_write_err = write(cli_sock, header, strlen(header));
		if (header_write_err < 0) {
			perror("Error: Writing the HTTP Header failed");
			close(cli_sock);
			close(serv_sock);
			return;
		}

		/* Wtite the rest of the body to socket */
		int body_write_err = write(cli_sock, file_buffer, file_len);
		if (body_write_err < 0) {
			perror("Error: Writing the file failed");
			close(cli_sock);
			close(serv_sock);
			return;
		}

		/* Cleanup */
		free(file_buffer);
		close(cli_sock);
	}

	close(serv_sock);

}

const char *get_content_type(const char *path) {
	const char *last_dot = strrchr(path, '.');
	if (last_dot) {
		if (strcmp(last_dot, ".html") == 0)
			return "text/html";

		if (strcmp(last_dot, ".css") == 0)
			return "text/css";
		
		if (strcmp(last_dot, ".js") == 0)
			return "application/javascript";
		
		if (strcmp(last_dot, ".png") == 0)
			return "image/png";
		
		if (strcmp(last_dot, ".jpg") == 0)
			return "image/jpeg";
	}
	return "text/plain";
}

size_t get_file_size(FILE *fp) {
	/* Go to the end of file */
	fseek(fp, 0, SEEK_END);

	/* Use ftell() to get the file length */
	size_t len = ftell(fp);

	/* Set pointer back to the beginning */
	fseek(fp, 0, SEEK_SET);

	/* Return value */
	return len;
}
