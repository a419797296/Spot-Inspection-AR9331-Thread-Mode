#ifndef OXYGEN_FLOW_H
#define OXYGEN_FLOW_H

#define REPORT_TIME_INTERVAL	2000   //ms

#define RESPONSE

void oxygenFlowInitSigaction(void);
void oxygenFlowInitTime(long ms);
void oxygenFlowTimer(int sig);
void oxygenFlowRun(void);
void oxygenFlowCfgTm(PT_Data_Info pt_data_info);
void oxygenFlowCfgServer(PT_Data_Info pt_data_info);
void oxygenFlowReport(PT_Data_Info pt_data_info);
void oxygenFlowUpdata(void);
int oxygenFlowPackage(char *conbined_buf, int *conbined_len, char *new_data, int new_data_len, char first_byte, int max_pkg_len);

#endif