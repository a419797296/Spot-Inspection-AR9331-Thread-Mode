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
#include "socket_server.h"
#include "socket_driver.h"
#include "socket_ser2net.h"
#include "wifi_config.h"

pthread_t threadID;
pthread_mutex_t db;
pthread_cond_t  db_update;
char *out_data;

int main(int argc,char *argv[])
{
  
  if( pthread_mutex_init(&db, NULL) != 0 )
  /* 初始化 global.db 成员 */
  {
    return -1;
  }
  if( pthread_cond_init(&db_update, NULL) != 0 )
  /* 初始化 global.db_update(条件变量) 成员 */
  {
    printf("could not initialize condition variable\n");
    return -1;
  }
  server_run(&threadID);
  out_data = malloc(1024);
  if (out_data ==NULL)
  {
    printf("malloc error\n");
  }

    while(1)
    {
      pthread_cond_wait(&db_update, &db);
      
      printf("recieved the data is %s---------------\n",out_data);
      if(!check_connectiong(5))
      	printf("the network is ready\n");
    }
}