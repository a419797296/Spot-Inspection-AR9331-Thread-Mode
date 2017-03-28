#ifndef OXYGEN_FLOW_H
#define OXYGEN_FLOW_H

#define REPORT_TIME_INTERVAL	2000   //ms

void oxygenFlowInitSigaction(void);
void oxygenFlowInitTime(long ms);
void oxygenFlowRun(void);
void oxygenFlowCfg(PT_Data_Info pt_data_info);
void oxygenFlowReport(int sig);
void oxygenFlowUpdata(void);

#endif