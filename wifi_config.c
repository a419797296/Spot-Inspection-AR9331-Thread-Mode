#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "wifi_config.h"
//-----------------------config wifi------------
int config_wifi(char *ssid,char *key)
{
    char cmd_buff[50];
    memset(cmd_buff,0,50);
    printf("ARRAY_SIZE(cmd_buff) is %d\n", 50);

    sprintf(cmd_buff,"uci set wireless.@wifi-iface[-1].ssid=%s",ssid);
    printf("%s\n", cmd_buff);
    system(cmd_buff);

    memset(cmd_buff,0,50);
    sprintf(cmd_buff,"uci set wireless.@wifi-iface[-1].key=%s",key);
    printf("%s\n", cmd_buff);
    system(cmd_buff);

    system("uci commit wireless");

    system("/etc/init.d/network restart");
    return 0;

}
//-----------------------check connect------------
int check_connectiong(int test_time)
{
  while(test_time--)
  {
    if (!system("www.baidu.com -c 1 > nul"))
    {
      return 0;    //successed
    }
    sleep(1);
  }
  return -1;
}