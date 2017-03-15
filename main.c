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
#include "socket_bussiness.h"
#include "wifi_config.h"


pthread_mutex_t db;
pthread_cond_t  db_update;
char *out_data;
PT_Data_Info pt_data_info;
T_Data_Info t_data_info;
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
  memset(&t_data_info, 0, sizeof(t_data_info));
  printf("11111111111111\n");
  t_data_info.data = malloc(1024);
    printf("2222222222222222\n");
  if (t_data_info.data ==NULL)
  {
    printf("malloc error\n");
  }
  server_run(&socket_server_threadID);
  printf("33333333333\n");
  

  printf("77777777777\n");

  socket_bussiness();
  return 0;
}