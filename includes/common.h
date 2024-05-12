#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <libgen.h>

#define BUFFER_SIZE		1024

#define MAX_NAME_SIZE	512

#define SEND_CMD_STR	"SEND "
#define GET_CMD_STR		"GET "

#define VALID_FORMAT			".tar.gz"


typedef enum {
    FALSE,
    TRUE
} bool_t;

extern char SEND_FILE_FLAG;
extern char GET_FILE_FLAG;


int send_file(int sockfd, int filefd);
int get_file(int sockfd, int filefd);

//tool.c
bool_t name_is_targz(char *path);

#endif