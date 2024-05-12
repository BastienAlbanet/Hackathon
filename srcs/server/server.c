

#include "server.h"


int init_server(int *server_fd, struct sockaddr_in *address) {
	
	// Creating socket file descriptor
    if ((*server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation KO");
        return (0);
    }
	printf("Socket creation OK\n");
	
	// Bind the socket to the port
	address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);
    if (bind(*server_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("Binding socket KO");
        return (0);
    }
	printf("Binding socket OK\n");
	
	// Listen for incoming connections
    if (listen(*server_fd, CONNECT_REQ_QUEUE_SIZE) < 0) {
        perror("Listening socket KO");
        return (0);
    }
	printf("Listening socket OK\n");	
	return (1);
}


int main() {
	int server_fd, index;
    struct sockaddr_in address;
    unsigned int addrlen = sizeof(address);
	int clients[MAX_CONNECTIONS];
	
	// Create folders if needed
	mkdir(DATAS_FOLDER, 0666);
	mkdir(TEMP_FOLDER, 0666);
	
	// Init server
	if (!init_server(&server_fd, &address)) {
		fprintf(stderr, "Server initialisation KO\n");
		return (1);
	}
	
	// Ignore SIGCHLD
    signal(SIGCHLD, SIG_IGN);
	
	// Init clients (-1 -> no client connected)
	for (int i = 0; i < MAX_CONNECTIONS; i++) {
		clients[i] = -1;
	}
    
	index = 0;
    while (1) {
        // Accept a new connection
		clients[index] = accept(server_fd, (struct sockaddr *)&address, &addrlen);
		if (clients[index] < 0) {
			perror("Accept connection KO");
		} else {
			if (fork() == 0) {
				close(server_fd);
				handle_client(&clients[index]);
				exit(0);
			} else {
				close(clients[index]);
			}
		}
		
		// Get new index
		while (clients[index]!=-1) {
			index = (index+1)%MAX_CONNECTIONS;
		}
    }
    
    return 0;
}