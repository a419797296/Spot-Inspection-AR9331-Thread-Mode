#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

extern int socket_client_fd;
extern pthread_t socket_client_threadID;
/* prototypes */
int client_stop(pthread_t threadID);
int client_run(pthread_t *threadID);


#endif