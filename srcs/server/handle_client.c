

#include "server.h"


int server_get_file(int sockfd) {
	int filefd;
	ssize_t bytes_received;
	char path[MAX_NAME_SIZE+(sizeof(TEMP_FOLDER)-1)+1] = TEMP_FOLDER;
	char file_name[MAX_NAME_SIZE] = {0};
	
	bytes_received = recv(sockfd, file_name, MAX_NAME_SIZE, 0);
	if (bytes_received != MAX_NAME_SIZE) {
		printf("Nope %ld != %d\n", bytes_received, MAX_NAME_SIZE);
	} else {
		printf("OK \"%s\"\n", file_name);
	}
	strcpy(path+(sizeof(TEMP_FOLDER)-1), file_name);
	if (name_is_targz(path) == FALSE) {
		return (-1);
	}
	filefd = open(path, O_RDWR|O_CREAT|O_TRUNC, 0644);
	if (filefd == -1) {
		perror("Error opening file");
		return (-1);
	}
	get_file(sockfd, filefd);
	
	close(filefd);
	gzip(file_name);
	return (0);
}

int server_send_file(int sockfd) {
	int filefd;
	ssize_t bytes_received;
	char path[MAX_NAME_SIZE+(sizeof(DATAS_FOLDER)-1)+1] = DATAS_FOLDER;
	
	bytes_received = recv(sockfd, path+(sizeof(DATAS_FOLDER)-1), MAX_NAME_SIZE, 0);
	if (bytes_received != MAX_NAME_SIZE) {
		printf("Nope %ld != %d\n", bytes_received, MAX_NAME_SIZE);
	} else {
		printf("OK \"%s\"\n", path);
	}
	filefd = open(path, O_RDONLY);
	if (filefd == -1) {
		perror("Error opening file");
		return (-1);
	}
	send_file(sockfd, filefd);
	
	close(filefd);
	return (0);
}

int handle_client(int *sockfd_ptr) {
	int sockfd = *sockfd_ptr;
	//char buffer[BUFFER_SIZE+1];
	char cmd;
    ssize_t bytes_received;

	bytes_received = recv(sockfd, &cmd, 1, 0);
	if (bytes_received != 1) {
		fprintf(stderr, "Invalide request\n");
		close(*sockfd_ptr);
		*sockfd_ptr = -1;
		return (-1);
	} else if (bytes_received == 0) {
		printf("Client disconnected\n");
		close(*sockfd_ptr);
		*sockfd_ptr = -1;
		return (0);
	}
	
	if (cmd == SEND_FILE_FLAG) {//if client send, server get
		printf("Receiving file\n");
		if (server_get_file(sockfd) == 0) {
			printf("Success\n");
		}
	} else if (cmd == GET_FILE_FLAG) {//if client get, server send
		printf("Sending file\n");
		if (server_send_file(sockfd) == 0) {
			printf("Success\n");
		}
	} else {
		fprintf(stderr, "Wrong command received: %c\n", cmd);
	}
	close(*sockfd_ptr);
	*sockfd_ptr = -1;
	return (0);
}