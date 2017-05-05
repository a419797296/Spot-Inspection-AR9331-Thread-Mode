#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "cJSON.h"
#include "socket_client.h"
#include "main.h"
#include "oxygen_flow.h"
#include "socket_driver.h"
#include "com_tools.h"


//------------------------------------------------
void oxygenFlowUpdata(void)
{


}


//------------------------------------------------
void oxygenFlowInitSigaction(void)
{
    struct sigaction tact;
    tact.sa_handler = oxygenFlowTimer;
    tact.sa_flags = 0;
    sigemptyset(&tact.sa_mask);
    sigaction(SIGALRM, &tact, NULL);
}

//------------------------------------------------
void oxygenFlowTimer(int sig)
{
    printf("It's the time to report .......\n");
    is_time_to_report = 1;
}
//------------------------------------------------
void oxygenFlowInitTime(long ms)
{
    struct itimerval value;
    value.it_value.tv_sec = ms / 1000;
    value.it_value.tv_usec = ms % 1000 * 1000;
    value.it_interval = value.it_value;
	DBG("-------system will set the timeinterval to %ld-----\n",value.it_value.tv_sec);
    setitimer(ITIMER_REAL, &value, NULL);
}
//------------------------------------------------
void oxygenFlowRun(void)
{
	char time_interval[6]={0};
	getSysUciCfg("spotInspection","para","interval",time_interval);
	
	oxygenFlowInitSigaction();
    	oxygenFlowInitTime(atoi(time_interval)*1000);
}
//------------------------------------------------
void oxygenFlowCfgTm(PT_Data_Info pt_data_info)
{
    char *receivedData = pt_data_info->data;
    cJSON *json;
    int tmInterval;
    json=cJSON_Parse(receivedData);
    tmInterval = cJSON_GetObjectItem(json,"timeInterval")->valueint;
    oxygenFlowInitTime(tmInterval*1000);
	setSysUciCfgNum("spotInspection","para","interval",tmInterval);
    cJSON_Delete(json);
}

//------------------------------------------------
void oxygenFlowCfgServer(PT_Data_Info pt_data_info)
{
    char *receivedData = pt_data_info->data;
    cJSON *json;
    int port;
	char *ip;
    json=cJSON_Parse(receivedData);
    port = cJSON_GetObjectItem(json,"port")->valueint;
	ip = cJSON_GetObjectItem(json,"ip")->valuestring;
	setSysUciCfgNum("spotInspection","cloud","port",port);
	setSysUciCfgStr("spotInspection","cloud","ip",ip);
    cJSON_Delete(json);
}

//------------------------------------------------
void oxygenFlowReport(PT_Data_Info pt_data_info)
{
    uint16 oxygen=0;
    uint16 flow=0;
    uint16 temp=0;
    cJSON *root;
    char *out;


    char *receivedData = pt_data_info->data;

	//------------------resolve the data
    oxygen=BUILD_UINT16(*(receivedData+3),*(receivedData+4));
    flow=BUILD_UINT16(*(receivedData+5),*(receivedData+6));
    temp=BUILD_UINT16(*(receivedData+7),*(receivedData+8));
    root=cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "jsonType", JSON_TYPE_OXYGEN_REPOART);
    cJSON_AddStringToObject(root, "productMac", produc_info.mac);
    cJSON_AddNumberToObject(root, "oxygen", oxygen);
    cJSON_AddNumberToObject(root, "flow", flow);
    cJSON_AddNumberToObject(root, "temp", temp);
    out=cJSON_PrintUnformatted(root);

//------------------------if network is closed, try connect again and  save the data	
    if(socketWrite(cloud_iface.fd, out, strlen(out)) == -1)  //send data success
    		{
		cloud_iface.fd = -1;
		cloud_iface.fd = socketConnect(cloud_iface.ip,cloud_iface.port); 

		    if (cloud_iface.fd == -1)
		    {
		        printf("----------------can not reconnect to the server-------------\n");
	//------------------------here will add the data saving function			
		        return ;
		    }
		else
			{
			    printf("----------------have reconnect to the server-------------\n");
				system("/root/led.sh led_on tp-link:blue:system");	//light on the led
				sendProductInfo(cloud_iface.fd);
				
	//------------------------here will add the function of sending the resaved data
		}
	}
	else
		is_time_to_report = 0;
    
    cJSON_Delete(root);
    free(out);
//---------------------reset the time_interval time
	char time_interval[6]={0};
	getSysUciCfg("spotInspection","para","interval",time_interval);
    	oxygenFlowInitTime(atoi(time_interval)*1000);
}


