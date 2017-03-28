#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "cJSON.h"
#include "socket_server.h"
#include "main.h"
#include "oxygen_flow.h"
#include "socket_driver.h"


//------------------------------------------------
void oxygenFlowUpdata(void)
{


}


//------------------------------------------------
void oxygenFlowInitSigaction(void)
{
    struct sigaction tact;
    tact.sa_handler = oxygenFlowReport;
    tact.sa_flags = 0;
    sigemptyset(&tact.sa_mask);
    sigaction(SIGALRM, &tact, NULL);
}
//------------------------------------------------
void oxygenFlowInitTime(long ms)
{
    struct itimerval value;
    value.it_value.tv_sec = ms / 1000;
    value.it_value.tv_usec = ms % 1000 * 1000;
    value.it_interval = value.it_value;
    setitimer(ITIMER_REAL, &value, NULL);
}
//------------------------------------------------
void oxygenFlowRun(void)
{
    oxygenFlowInitSigaction();
    oxygenFlowInitTime(REPORT_TIME_INTERVAL);
}
//------------------------------------------------
void oxygenFlowCfg(PT_Data_Info pt_data_info)
{
    char *receivedData = t_data_info.data;
    cJSON *json;
    int timeInterval;
    json=cJSON_Parse(receivedData);
    timeInterval = cJSON_GetObjectItem(json,"timeInterval")->valueint;
    oxygenFlowInitTime(timeInterval);
    cJSON_Delete(json);
}
//------------------------------------------------
void oxygenFlowReport(int sig)
{
    uint16 oxygen=0;
    uint16 flow=0;
    uint16 temp=0;
    cJSON *root;
    char *out;
    printf("reporting .......\n");
    pthread_mutex_lock(&db);
    char *receivedData = t_data_info.data;
    if (t_data_info.type != JSON_TYPE_OXYGEN_REPOART)
    {
        printf("err:unmatched the jsonType .......\n");
        pthread_mutex_unlock( &db );// 原子操作结束
        return;
    }
    pthread_mutex_unlock( &db );// 原子操作结束

    oxygen=BUILD_UINT16(*(receivedData+3),*(receivedData+4));
    flow=BUILD_UINT16(*(receivedData+5),*(receivedData+6));
    temp=BUILD_UINT16(*(receivedData+7),*(receivedData+8));
    root=cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "jsonType", JSON_TYPE_OXYGEN_REPOART);
    cJSON_AddNumberToObject(root, "productMac", produc_info.mac);
    cJSON_AddNumberToObject(root, "oxygen", oxygen);
    cJSON_AddNumberToObject(root, "flow", flow);
    cJSON_AddNumberToObject(root, "temp", temp);
    out=cJSON_PrintUnformatted(root);
    socketWrite(server_iface.fd, out, strlen(out)+1);   //+1 means sent '/0' together
    cJSON_Delete(root);
    free(out);
}


