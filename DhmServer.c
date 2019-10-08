#include <stdio.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	setbuf(stdout, NULL);
	if (argc != 2) {
		printf("USAGE: dhm_server.exe [PORT_NUMBER]\n");
	}

	int port_num = strtoumax(argv[1], NULL, 10);

	if (port_num == 0) {
		printf("Invalid port number.\nUSAGE: dhm_server.exe [PORT_NUMBER]\n");
	}

	int sock_handle;
	struct sockaddr_in address = (const struct sockaddr_in){0 };; // IPv4 address
	int addr_len = sizeof(address);

	if ((sock_handle = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket creation failed");
		return -1;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port_num); // Convert the byte order?

	if (bind(sock_handle, (struct sockaddr *) &address, sizeof(address)) < 0) {
		perror("Socket bind failed");
		return -2;
	}

	if (listen(sock_handle, 5) < 0) {
		perror("Listen failed?");
		return -3;
	}

	int new_socket;

//	if ((new_socket = accept(sock_handle, (struct sockaddr *) &address, (socklen_t *) &addr_len)) < 0) {
//		perror("Failed to accept");
//		return -4;
//	}

	printf("Started listening on %d\n", port_num);

	char buffer[256] = {0};

	while ((new_socket = accept(sock_handle, (struct sockaddr *) &address, (socklen_t *) &addr_len)) >= 0) {
		while (1) {

//			int error = 0;
//			socklen_t len = sizeof (error);
//			int retval = getsockopt (new_socket, SOL_SOCKET, SO_ERROR, &error, &len);
//
//			printf("retval: %d -- errorval: %d \n", retval, error);

			int num_read = recv(new_socket, buffer, 256, 0);
			buffer[num_read] = '\0';

			if (num_read == 0) {
				break;
			}

			printf("Received: %s\n", buffer);

			if (send(new_socket , buffer , num_read , MSG_NOSIGNAL) == -1) {
				break;
			}
		}
	}

	close(sock_handle);
	return 0;
}