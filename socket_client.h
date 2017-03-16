#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

extern int socket_client_fd;
extern pthread_t socket_client_threadID;

#define SERVER_IP	"120.27.134.154"
#define	SERVER_PORT	6000
/* prototypes */
int client_stop(pthread_t threadID);
int client_run(pthread_t *threadID);


#endif