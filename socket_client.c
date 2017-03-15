#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <netdb.h>
#include <errno.h>
#include <limits.h>
#include "socket_client.h"
#include "socket_driver.h"
#include "main.h"

int socket_client_fd;
pthread_t socket_client_threadID;
/***********local fucntion***********/
static void *client_thread(void *arg);
static void client_cleanup(void *arg);

int client_init(int id)
{

    return 0;
}
/******************************************************************************
Description.: this will stop the server thread, client threads
              will not get cleaned properly, because they run detached and
              no pointer is kept. This is not a huge issue, because this
              funtion is intended to clean up the biggest mess on shutdown.
Input Value.: id determines which server instance to send commands to
Return Value: always 0
******************************************************************************/
int client_stop(pthread_t threadID)
{

    printf("will cancel server thread #%02d\n", (int)threadID);
    pthread_cancel(threadID);

    return 0;
}

/******************************************************************************
Description.: This creates and starts the server thread
Input Value.: id determines which server instance to send commands to
Return Value: always 0
******************************************************************************/
int client_run(pthread_t *threadID)
{
    printf("launching client thread #%02d\n", (int)(*threadID));

    pthread_create(threadID, NULL, client_thread, (void *)NULL);
    pthread_detach(*threadID);

    return 0;
}

static void *client_thread(void *arg)
{
    pthread_cleanup_push(client_cleanup, "test");

    printf("----------------here is in the client thread-------------\n");

    pthread_cleanup_pop(1);
    return NULL;
}

/******************************************************************************
Description.: This function cleans up ressources allocated by the server_thread
Input Value.: arg is not used
Return Value: -
******************************************************************************/
static void client_cleanup(void *arg)
{

    printf("cleaning up ressources allocated by client thread: %s\n",(char *)arg);
}