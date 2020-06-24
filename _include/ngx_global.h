#ifndef __NGX_GBLDEF_H__
#define __NGX_GBLDEF_H__

#include <signal.h>

typedef struct _CConfItem
{
	char ItemName[50];
	char ItemContent[500];
}CConfItem,*LPCConfItem;

//日志相关
typedef struct
{
    int log_level;
    int fd;
}ngx_log_t;

extern size_t       g_argvneedmem;
extern size_t       g_envneedmem;
extern int          g_os_argc;
extern char         **g_os_argv;
extern char         *gp_envmem;
extern int          g_daemonized;

extern pid_t        ngx_pid;
extern pid_t        ngx_parent;
extern ngx_log_t    ngx_log;
extern int          ngx_process;
extern sig_atomic_t ngx_reap;

#endif