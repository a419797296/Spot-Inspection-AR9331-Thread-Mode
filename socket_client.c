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
#include "com_tools.h"
#include "main.h"
#include "oxygen_flow.h"

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
	oxygenFlowRun();
    pthread_create(threadID, NULL, client_thread, (void *)NULL);
    pthread_detach(*threadID);

    return 0;
}

static void *client_thread(void *arg)
{
    char buff[1024]={0};
    int nbyte;
    pthread_cleanup_push(client_cleanup, "test");
    socket_client_fd = socketConnect(SERVER_IP,SERVER_PORT); 

    if (socket_client_fd == -1)
    {
        printf("----------------can not connect to the server-------------\n");
        return NULL;
    }
    printf("----------------have connect to the server-------------\n");
    sendProductInfo(socket_client_fd);
    while(1)
    {
      if((nbyte=read(socket_client_fd,buff,1024))<=0)
      {
        printf("client read err---------------\n");
        close(socket_client_fd);
        return NULL;
      }  
      buff[nbyte] = 0;
      printf("nbyte is %d,read data is %s\n",nbyte,buff);
      pthread_mutex_lock(&db);
      memcpy(t_data_info.data, buff, nbyte+1);
      t_data_info.length = nbyte;
      t_data_info.orig_fd = socket_client_fd;
      pthread_cond_broadcast(&db_update);// 发出一个数据更新的信号，通知发送通道来取数据
      pthread_mutex_unlock( &db );// 原子操作结束

    }

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