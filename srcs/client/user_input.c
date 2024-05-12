
#include "common.h"
#include "client.h"

/*
int client_send_file(int sockfd, char *path) {
	char *file_name;
	
	if (strlen(path) < 1) {
		fprintf(stderr, "No path given\n");
		return (0);
	}
	
	if (send_flag(sockfd, SEND_FILE_FLAG) == -1) {
		return (-1);
	}

	if (wait_flag(sockfd, VALIDATOR_FLAG, FALSE) == -1) {
		return (-1);
	}
	
	if (extract_file_name(path, &file_name) == -1) {
		send_flag(sockfd, CANCEL_FLAG);
		return (-1);
	}
	
	if (send_file_name(sockfd, file_name) == -1) {
		return (-1);
	}
	
	if (send_file(sockfd, path) == -1) {
		return (-1);
	}
	return (0);
}

int client_get_file(int sockfd, char *file_name) {
	
	if (strlen(file_name) < 1) {
		fprintf(stderr, "No file name given\n");
		return (0);
	}
	
	if (send_flag(sockfd, GET_FILE_FLAG) == -1) {
		return (-1);
	}
	
	if (wait_flag(sockfd, VALIDATOR_FLAG, FALSE) == -1) {
		return (-1);
	}
	
	if (send_file_name(sockfd, file_name) == -1) {
		return (-1);
	}
	
	if (get_file(sockfd, file_name, O_WRONLY|O_CREAT|O_EXCL) == -1) {
		return (-1);
	}
	
	return (0);
}

int handle_user_input(int sockfd) {
	char buffer[BUFFER_SIZE+1];
	
	while (1) {
		memset(buffer, 0, sizeof(buffer));
		printf("Command: ");
		if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
			return (0);
		}
		
		// Replace \n by '\0'
		char *ptr = strchr(buffer, '\n');
		if (ptr == NULL) {
			ptr = buffer;
		}
		*ptr = '\0';
		
		if (strncasecmp("send ", buffer, 5) == 0) {
			if (client_send_file(sockfd, buffer+5) == 0) {
				printf("Success\n");
			}
		} else if (strncasecmp("get ", buffer, 4) == 0) {
			if (client_get_file(sockfd, buffer+4) == 0) {
				printf("Success\n");
			}
		} else {
			fprintf(stderr, "Valid commands:\n  send FILE_PATH\n  get FILE_PATH\n");
		}
		
		
		
	}
	return (0);
}*/
