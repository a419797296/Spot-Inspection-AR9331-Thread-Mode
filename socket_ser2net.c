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
#include "socket_ser2net.h"
#include "socket_driver.h"
#include "com_tools.h"
#include "main.h"

int socket_ser2net_fd;
pthread_t socket_ser2net_threadID;
/***********local fucntion***********/
static void *ser2net_thread(void *arg);
static void ser2net_cleanup(void *arg);

int ser2net_init(int id)
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
int ser2net_stop(pthread_t threadID)
{

    printf("will cancel ser2net thread #%02d\n", (int)threadID);
    pthread_cancel(threadID);

    return 0;
}

/******************************************************************************
Description.: This creates and starts the server thread
Input Value.: id determines which server instance to send commands to
Return Value: always 0
******************************************************************************/
int ser2net_run(pthread_t *threadID)
{
    printf("launching ser2net thread #%02d\n", (int)(*threadID));

    pthread_create(threadID, NULL, ser2net_thread, (void *)NULL);
    pthread_detach(*threadID);

    return 0;
}

static void *ser2net_thread(void *arg)
{
    char buff[1024]={0};
    char package[SER2NET_PACK_NUMS];
    int  nbyte;
    int  allNum=0;
    bool fullPackaged = 0;


    pthread_cleanup_push(ser2net_cleanup, "test");
    socket_ser2net_fd = socketConnect(SER2NET_IP,SER2NET_PORT); 

    if (socket_ser2net_fd == -1)
    {
        printf("----------------can not connect to the ser2net server-------------\n");
        return NULL;
    }
    printf("----------------have connect to the ser2net server-------------\n");
    while(1)
    {
      if((nbyte=read(socket_ser2net_fd,package,1024))<=0)
      {
        printf("ser2net read err---------------\n");
        close(socket_ser2net_fd);
        return NULL;
      }  
      memcpy(buff + allNum, package, nbyte);
      allNum += nbyte;

#ifdef SER2NET_END_WITH_NULL
      if (package[nbyte - 1] == '\0')   //读到的最后一位不为零, 读取完毕
      {
          fullPackaged = 1;
      }
#endif

#ifdef SER2NET_END_WITH_LENGTH

      if(buff[0]!=0x16)
      {
          PLOG("first byte error :%x\n",buff[0]);
          allNum = 0;
          // memset(buff, '\0', sizeof(buff));
          continue;
      }       
        
      if(allNum >= SER2NET_FIX_LENGTH)
      {
        int i=0;
        unsigned char xor=0;
        for(i=0; i<allNum; i++)
        {
            PLOG("%02x ",(unsigned char)buff[i]);
            if(i<11)
                xor-=buff[i];

        }
        PLOG("\nthe xor is %02x \n",xor);
        /*PLOG("\nthe soc is %02x \n",soc);*/
        if(xor==(unsigned char)buff[11])
        {
            fullPackaged =1;
        }
        else
        {
            allNum = 0;
            // memset(buff, '\0', sizeof(buff));
        }
      }
#endif
      if(fullPackaged)
      {
        fullPackaged = 0;
        buff[allNum] = 0;
        printf("allNum is %d,read data is %s\n",allNum,buff);
        pthread_mutex_lock(&db);
        memcpy(t_data_info.data, buff, allNum+1);
        t_data_info.length = allNum;
        t_data_info.orig_fd = socket_ser2net_fd;
        pthread_cond_broadcast(&db_update);// 发出一个数据更新的信号，通知发送通道来取数据
        pthread_mutex_unlock( &db );// 原子操作结束        
      }
    }

    pthread_cleanup_pop(1);
    return NULL;
}

/******************************************************************************
Description.: This function cleans up ressources allocated by the server_thread
Input Value.: arg is not used
Return Value: -
******************************************************************************/
static void ser2net_cleanup(void *arg)
{

    printf("cleaning up ressources allocated by ser2net thread: %s\n",(char *)arg);
}