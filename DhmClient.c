#include <stdio.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
	setbuf(stdout, NULL);
	if (argc != 2) {
		printf("USAGE: dhm_client.exe [PORT_NUMBER]\n");
	}

	int port_num = strtoumax(argv[1], NULL, 10);

	if (port_num == 0) {
		printf("Invalid port number.\nUSAGE: dhm_server.exe [PORT_NUMBER]\n");
	}

	int sock_handle;
	struct sockaddr_in address = (const struct sockaddr_in){0 }; // IPv4 Address
	int addr_len = sizeof(address);

	if ((sock_handle = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket creation failed");
		return -1;
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(port_num);

	if (connect(sock_handle, (struct sockaddr *) &address, sizeof(address)) != 0) {
		perror("Socket connect failed");
		return -2;
	}

	char in_buffer[256] = {0};
	char out_buffer[256] = {0};

	while (1) {

		if (fgets(out_buffer, 12, stdin) == NULL) {
			return 0;
		}

		size_t len = strlen(out_buffer);

		int sent_bytes = send(sock_handle, out_buffer, len + 1, 0);

		int recv_bytes = recv(sock_handle, in_buffer, 256, 0);

		in_buffer[recv_bytes] = '\0';

		printf("Server: %s\n", in_buffer);
	}


	close(sock_handle);

	return 0;
}