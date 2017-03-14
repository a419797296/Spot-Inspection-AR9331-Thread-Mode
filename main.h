#ifndef MAIN_H
#define MAIN_H

#define _DEBUG_TRACE_CMH_	2  

  
#if 1==_DEBUG_TRACE_CMH_    //普通打印  
    #define TRACE_CMH printf  
#elif 2==_DEBUG_TRACE_CMH_  //打印文件名、行号  
    #define TRACE_CMH(fmt,...)	printf("%s(%d): "##fmt, __FILE__, __LINE__, ##__VA_ARGS__)  
#elif 3==_DEBUG_TRACE_CMH_  //打印文件名、行号、函数名  
    #define TRACE_CMH(fmt,...)	printf("%s(%d)-<%s>: "##fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)  
#else  
    #define TRACE_CMH  
#endif //_TRACE_CMH_DEBUG_  


#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof((arr)[0]) 

/*******************************************************/  
// #include <pthread.h>
// typedef struct Server {
//   int 				fd;
//   pthread 			threadID;
//   pthread_mutex_t 	db;
//   pthread_cond_t  	db_update;
// }T_Server, *PT_Servrer;

#define DEBUG

#ifndef DBG
#ifdef DEBUG
#define DBG(...) printf(__VA_ARGS__)
#else
#define DBG(...)
#endif
#endif

#define OUTPUT_PLUGIN_PREFIX " o: "
#define OPRINT(...) { char _bf[1024] = {0}; snprintf(_bf, sizeof(_bf)-1, __VA_ARGS__); fprintf(stderr, "%s", OUTPUT_PLUGIN_PREFIX); fprintf(stderr, "%s", _bf); syslog(LOG_INFO, "%s", _bf); }

extern pthread_mutex_t db;
extern pthread_cond_t  db_update;
extern char *out_data;


#endif