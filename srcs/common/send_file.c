
#include "common.h"

int send_file(int sockfd, int filefd) {
	ssize_t size;
	char buffer[BUFFER_SIZE+1] = {0};
	
	while ((size = read(filefd, buffer, BUFFER_SIZE)) > 0) {
		if (send(sockfd, buffer, size, 0) == -1) {
			perror("Error send file data");
			return (-1);
		}
	}
	printf("Sending file ok\n");
	return (0);
}




















