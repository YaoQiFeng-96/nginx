#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "ngx_c_conf.h"
#include "ngx_func.h"
#include "ngx_global.h"
#include "ngx_macro.h"

//释放资源
static void freeresource();

size_t       g_argvneedmem = 0;
size_t       g_envneedmem = 0;
int          g_os_argc;
char         **g_os_argv;
char         *gp_envmem = nullptr;
int          g_daemonized = 0;

pid_t        ngx_pid;
pid_t        ngx_parent;
int          ngx_process;
sig_atomic_t ngx_reap;

int main(int argc, char* const *argv)
{
    ngx_pid = getpid();
    ngx_parent = getppid();

    g_daemonized = 0;
    for(int i = 0; i < argc; i++)
    {
        g_argvneedmem += strlen(argv[i]) + 1;
    }
    for(int i = 0; environ[i]; i++)
    {
        g_envneedmem += strlen(environ[i]) + 1;
    }
    g_os_argc = argc;
    g_os_argv = (char **)argv;

    ngx_log.fd = -1;
    ngx_process = NGX_PROCESS_MASTER;
    ngx_reap = 0;


    CConfig *p_config = CConfig::GetInstance();
    if(p_config->Load("nginx.conf") == false)          
    {   
        ngx_log_init();
        ngx_log_stderr(0,"配置文件[%s]载入失败，退出!","nginx.conf");
    }

    ngx_log_init();

    if(ngx_init_signals() != 0)
    {
        goto lblexit;
    }

    ngx_init_setproctitle();

    if(p_config->GetIntDefault("Daemon",0) == 1)
    {
        int cdaemonresult = ngx_daemon();
        if(cdaemonresult == -1)
        {
            goto lblexit;
        }
        if(cdaemonresult == 1)
        {
            freeresource();
            return 0;
        }
        g_daemonized = 1;
    }
    ngx_master_process_cycle();

lblexit:
    ngx_log_stderr(0,"程序退出.");
    freeresource();
    return 0;
}

void freeresource()
{
    if(ngx_log.fd != STDERR_FILENO && ngx_log.fd != -1)  
    {        
        close(ngx_log.fd);
        ngx_log.fd = -1;      
    }
}