#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

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

#define  MAX_SD_LEN   50
/* store configuration for each server instance */
typedef struct {
    int port;
    char *credentials;
    char *www_folder;
    char nocommands;
} config;

/* context of each server thread */
typedef struct {
    int sd[MAX_SD_LEN];
    int sd_len;
    int id;
    pthread_t threadID;

    config conf;
} context;

/*
 * this struct is just defined to allow passing all necessary details to a worker thread
 * "cfd" is for connected/accepted filedescriptor
 */
typedef struct {
    context *pc;
    int fd;
} cfd;

/* prototypes */
int server_stop(int id);
int server_run(int id);
void *server_thread(void *arg);
void *client_thread(void *arg);
void server_cleanup(void *arg);

#endif