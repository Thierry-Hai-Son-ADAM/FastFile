#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void) {
	//a file descriptor is returned after creating a TCP socket
	int socket_fd = socket(AF_INET , SOCK_STREAM , 0);

	if (socket_fd == -1) {
		perror("socket");
		return 1;
	}

	//Instantiating a sockaddr_in struct using internet socket adress
	struct sockaddr_in server_addr = {0};

	//storing binary localhost ipadress into sin_addr field
	int inet_pton_ret_val = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

	if (inet_pton_ret_val == -1) {
		perror("inet_pton");
		close(socket_fd);
		return 1;
	}
	else if (inet_pton_ret_val == 0) {
		fprintf(stderr, "Invalid ip address\n");
		close(socket_fd);
		return 1;
	}

	server_addr.sin_family = AF_INET;
	//converting integer 8080 to the network endianness (from little endian to big endian)
	server_addr.sin_port = htons(8080);
	
	//binding 127.0.0.1:8080 to the socket ressource (socket_fd)
	int bind_ret_val = bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

	if (bind_ret_val == -1) {
		perror("bind");
		close(socket_fd);
		return 1;
	}

	int listen_val = listen(socket_fd, 0);

	if (listen_val == -1) {
		perror("listen");
		close(socket_fd);
		return 1;
	}
	struct sockaddr_in client_addr = {0};

	socklen_t client_addr_size = sizeof(client_addr);
	
	int client_socket_fd = accept(socket_fd, (struct sockaddr *) &client_addr, &client_addr_size);
	if (client_socket_fd == -1) {
		perror("accept");
		close(socket_fd);
		return 1;
	}
	

	char ip_address_client[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &client_addr.sin_addr, ip_address_client, sizeof(ip_address_client));

	printf("client_addr_size %u bytes\n", client_addr_size);
	printf("client_addr ip address: %s\n", ip_address_client);

	char msg[] = "I'm using the standardized POSIX interface implemented by the kernel of this OS"
	"(Ubuntu) which is Linux to send this message";

	int send_nb_bytes = send(client_socket_fd, &msg, sizeof(msg), MSG_NOSIGNAL);
	if (send_nb_bytes == -1) {
		perror("send");
		close(socket_fd);
		close(client_socket_fd);
		return 1;
	}

	printf("send() number of bytes sent: %d\n", send_nb_bytes);

	close(socket_fd);
	close(client_socket_fd);

	return 0;

}

