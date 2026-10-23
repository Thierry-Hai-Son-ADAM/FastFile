#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void) {
	//a file descriptor is returned after creating a TCP socket
	int socket_fd = socket(AF_INET , SOCK_STREAM , 0);

	printf("Socket ressource file descriptor number is %d\n", socket_fd);

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
		printf("src does not contain a character string representing a valid network "
			"address in the specified address family"); //from man doc
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
	
	close(socket_fd);
	
	return 0;

}

