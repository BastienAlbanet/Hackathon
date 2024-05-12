#ifndef SERVER_H_
#define SERVER_H_

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include "zlib.h"

#include "common.h"

#define PORT 8080

#define CONNECT_REQ_QUEUE_SIZE	1024
#define MAX_CONNECTIONS			128

#define DATAS_FOLDER			"datas/"




int handle_client(int *sockfd_ptr);


#endif