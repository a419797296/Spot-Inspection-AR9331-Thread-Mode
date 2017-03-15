#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H


#define  MAX_SD_LEN   50
extern int socket_server_fd;
extern pthread_t socket_server_threadID;
/* store configuration for each server instance */
typedef struct {
    int port;
    char *credentials;
    char *www_folder;
    char nocommands;
} config;

/* context of each server thread */
typedef struct {
    int sd[MAX_SD_LEN];
    int sd_len;
    int id;
    pthread_t threadID;

    config conf;
} context;

/*
 * this struct is just defined to allow passing all necessary details to a worker thread
 * "cfd" is for connected/accepted filedescriptor
 */
typedef struct {
    context *pc;
    int fd;
} cfd;

/* prototypes */
int server_stop(pthread_t threadID);
int server_run(pthread_t *threadID);


#endif