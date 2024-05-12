
#include "common.h"

int get_file(int sockfd, int filefd) {
	ssize_t bytes_received;
	char buffer[BUFFER_SIZE+1];
	
	while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0))) {
		if (bytes_received == -1) {
			perror("recv");
			return (-1);
		}
		write(filefd, buffer, bytes_received);
	}
	return (0);
}

