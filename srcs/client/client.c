

#include "client.h"


int connect_to_server(int *sockfd) {
    struct sockaddr_in serv_addr;
    
    // Create socket
    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
    
    // Connect to server
    if (connect(*sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    //handle_user_input(sockfd);
	fprintf(stderr, "Connection ok\n");
    
	return (0);
}

int client_send(char *file_path) {
	char file_name[MAX_NAME_SIZE+1] = {0};
	int sockfd;
	int filefd;
	char *file_name_ptr;
	
	if (name_is_targz(file_path) == FALSE) {
		return (-1);
	}
	filefd = open(file_path, O_RDONLY);
	if (filefd == -1) {
		perror("Error opening file");
		return (-1);
	}
	
	//connect to server
	connect_to_server(&sockfd);
	
	//sending send file request
	send(sockfd, &SEND_FILE_FLAG, 1, 0);
	
	//exctract file_name
	file_name_ptr = basename(file_path);
	if (!file_name_ptr) {
		fprintf(stderr, "Error basename \"%s\"\n", file_path);
		close(filefd);
		close(sockfd);
		return (-1);
	}
	strncpy(file_name, file_name_ptr, MAX_NAME_SIZE);
	
	//send file_name
	send(sockfd, file_name, MAX_NAME_SIZE, 0);
	
	send_file(sockfd, filefd);
	
	close(filefd);
	close(sockfd);
	return (0);
}

int client_get(char *file_name_ptr) {
	char file_name[MAX_NAME_SIZE+1] = {0};
	int sockfd;
	int filefd;	
	
	filefd = open(file_name_ptr, O_RDWR|O_CREAT|O_TRUNC, 0644);
	if (filefd == -1) {
		perror("Error opening file");
		return (-1);
	}
	
	//connect to server
	connect_to_server(&sockfd);
	
	//sending send file request
	send(sockfd, &GET_FILE_FLAG, 1, 0);
	
	strncpy(file_name, file_name_ptr, MAX_NAME_SIZE);
	//send file_name
	send(sockfd, file_name, MAX_NAME_SIZE, 0);
	
	get_file(sockfd, filefd);
	
	close(filefd);
	close(sockfd);
	return (0);
}

int main() {
	char cmd_buffer[CMD_BUFFER_SIZE+1] = {0};
	char *ptr;
	
	while (1) {
		printf("Commands: ");
		if(!fgets(cmd_buffer, CMD_BUFFER_SIZE, stdin)) {
			perror("fgets Failed");
			continue;
		}
		if (!(ptr = strchr(cmd_buffer, '\n'))) {
			fprintf(stderr, "Command too long\n");
			continue;
		}
		*ptr = '\0';
		
		if (!strncasecmp(cmd_buffer, SEND_CMD_STR, sizeof(SEND_CMD_STR)-1)) {
			printf("send: \"%s\"\n", cmd_buffer);
			client_send(cmd_buffer+(sizeof(SEND_CMD_STR)-1));
		} else if (!strncasecmp(cmd_buffer, GET_CMD_STR, sizeof(GET_CMD_STR)-1)) {
			printf("Get: \"%s\"\n", cmd_buffer);
			client_get(cmd_buffer+(sizeof(GET_CMD_STR)-1));
		} else {
			
			printf("nope: \"%s\"\n", cmd_buffer);
		}
	}
	
    return 0;
}





