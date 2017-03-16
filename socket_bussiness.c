#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "socket_driver.h"
#include "socket_client.h"
#include "socket_server.h"
#include "socket_ser2net.h"
#include "socket_bussiness.h"
//#include "ad7606_app.h"
#include "com_tools.h"
#include "cJSON.h"
#include "main.h"
#include "wifi_config.h"


int socket_bussiness(void)
{


    while(1)
    {
        pthread_cond_wait(&db_update, &db);
      
        printf("recieved the data is %s---------------\n",t_data_info.data);
        doit(&t_data_info);

    }
    return 0;
}

//--------------------------判断是否是json格式---------
jsonType judgeJsonType(PT_Data_Info pt_data_info)
{
    cJSON *json;
    static jsonType json_type;
    char *receivedData = pt_data_info->data;
    int dataLength = pt_data_info->length;
    if(*receivedData=='{')
    {
        json=cJSON_Parse(receivedData);
        if (!json)
        {
            printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        }
        else
        {
            json_type = cJSON_GetObjectItem(json,"jsonType")->valueint;
            cJSON_Delete(json);
        }
    }
    else if(*receivedData==0x16)
        json_type = JSON_TYPE_OXYGEN;
    else
    {
        if (receivedData[0]=='$' && receivedData[1]=='@' && receivedData[dataLength-2]=='\r' && receivedData[dataLength-1]=='\n')
        {
            printf("------------------success+++success--%d------------\n",dataLength);
            json_type = JSON_TYPE_GETWAY_TO_ZIGBEE;
        }
        else
        {
            printf("------------------ERROR+++success--%d------------\n",dataLength);
            json_type = JSON_TYPE_ERROR;
        }
    }
    return json_type;
}

/* Parse text to JSON, then render back to text, and print! */
void doit(PT_Data_Info pt_data_info)
{

    pt_data_info->type=judgeJsonType(pt_data_info);
    switch(pt_data_info->type)
    {
    case JSON_TYPE_ERROR:
        printf("jsonType is ERROR\r\n");
        break;
    // case JSON_TYPE_CONTROL_CMD:
    //     printf("jsonType is CONTROL_CMD\r\n");
    //     doControlInfo(receivedData);
    //     break;
    // case JSON_TYPE_GETWAY_TO_ZIGBEE:
    //     printf("jsonType is GETWAY_TO_ZIGBEE\r\n");
    //     socketWriteByPackages(socket_ser2net_interface.socket_fd,receivedData, dataLength, 64, 20000);
    //     break;
    // case JSON_TYPE_ZIGBEE_TO_GETWAY:
    //     printf("jsonType is ZIGBEE_TO_GETWAY\r\n");
    //     *(receivedData+dataLength)=0;
    //     if (*(enable_mode+1)=='1')//if the app works in server model, then send the data back to client
    //     {
    //         socketWriteNoEnd(socket_server_interface.socket_fd, receivedData, dataLength+1);   //+1 means sent '/0' together
    //         printf("%c\n", *(enable_mode+1));
    //     }

    //     if (*(enable_mode+2)=='1')//if the app works in client model, then send the data back to server
    //         socketWriteNoEnd(socket_client_interface.socket_fd, receivedData, dataLength+1);   //+1 means sent '/0' together
    //     break;
    // case JSON_TYPE_DATA_REPOART:
    //     printf("jsonType is DATA_REPOART\r\n");
    //     doDataReport(receivedData);
    //     break;
    // case JSON_TYPE_OXYGEN:
    //     printf("jsonType is JSON_TYPE_OXYGEN\r\n");
    //     oxygenFlowReport(receivedData);
    //     break;
    case JSON_TYPE_WIFI_CONFIG:
        printf("jsonType is JSON_TYPE_WIFI_CONFIG\r\n");
        if (config_wifi(pt_data_info)==-1)
        {
            printf("---------------unconnect-----\n");
            write(pt_data_info->orig_fd,"unconnect",sizeof("unconnect"));
        }
        else
        {
            printf("---------------connect-----\n");
            write(pt_data_info->orig_fd,"connect",sizeof("connect"));
            client_run(&socket_client_threadID);            
        }

        break;
    default:
        printf("jsonType is default+\r\n");
        break;
    }
}