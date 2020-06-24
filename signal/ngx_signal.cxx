#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#include "ngx_c_conf.h"
#include "ngx_func.h"
#include "ngx_global.h"
#include "ngx_macro.h"

typedef struct
{
    int         signo;
    const char  *signame;
    void  (*handler)(int signo, siginfo_t *siginfo, void *ucontext);
}ngx_signal_t;

static void ngx_signal_handler(int signo, siginfo_t *siginfo, void *ucontext);
static void ngx_process_get_status(void);

ngx_signal_t    signals[] = {
    { SIGHUP,       "SIGHUP",           ngx_signal_handler},
    { SIGINT,       "SIGINT",           ngx_signal_handler},
    { SIGTERM,      "SIGTERM",          ngx_signal_handler},
    { SIGCHLD,      "SIGCHLD",          ngx_signal_handler},
    { SIGQUIT,      "SIGQUIT",          ngx_signal_handler},
    { SIGIO,        "SIGIO",            ngx_signal_handler},
    { SIGSYS,       "SIGSYS,SIG_IGN",   NULL},
    { 0,            NULL,               NULL}
};

int ngx_init_signals()
{
    ngx_signal_t        *sig;
    struct sigaction    sa;

    for(sig = signals; sig->signo != 0; sig++)
    {
        memset(&sa, 0, sizeof(struct sigaction));

        if(sig->handler)
        {
            sa.sa_sigaction = sig->handler;
            sa.sa_flags = SA_SIGINFO;
        }
        else
        {
            sa.sa_handler = SIG_IGN;
        }
        sigemptyset(&sa.sa_mask);

        if(sigaction(sig->signo, &sa, NULL) == -1)
        {
            ngx_log_error_core(NGX_LOG_EMERG,errno,"sigaction(%s) failed",sig->signame);
            return -1;
        }
        else
        {
            ngx_log_stderr(0,"sigaction(%s) succed!",sig->signame);
        }
    }
    return 0;
}

static void ngx_signal_handler(int signo, siginfo_t *siginfo, void *ucontext)
{
    ngx_signal_t    *sig;
    char            *action;

    for(sig = signals; sig->signo != 0; sig++)
    {
        if(sig->signo == signo)
            break;
    }
    action = (char *)"";

    if(ngx_process == NGX_PROCESS_MASTER)
    {
        switch(signo)
        {
        case SIGCHLD:
            ngx_reap = 1;
            break;
        default:
            break;
        }
    }
    else if(ngx_process == NGX_PROCESS_WORKER)
    {

    }
    else
    {
    
    }
    
    if(siginfo && siginfo->si_pid)
    {
        ngx_log_error_core(NGX_LOG_NOTICE,0,"signal %d (%s) received from %P%s", signo, sig->signame, siginfo->si_pid, action); 
    }
    else
    {
        ngx_log_error_core(NGX_LOG_NOTICE,0,"signal %d (%s) received %s",signo, sig->signame, action);//没有发送该信号的进程id，所以不显示发送该信号的进程id
    }

    if(signo == SIGCHLD)
    {
        ngx_process_get_status();
    }
    return;
}

static void ngx_process_get_status(void)
{
    pid_t   pid;
    int     status;
    int     err;
    int     one = 0;

    for( ;; )
    {
        pid = waitpid(-1, &status, WNOHANG);

        if(0 == pid)
            return;

        if(-1 == pid)
        {
            err = errno;
            if(err == EINTR)
                continue;
            
            if(err == ECHILD && one)
                return;

            if(err == ECHILD)
            {
                ngx_log_error_core(NGX_LOG_INFO,err,"waitpid() failed!");
                return;
            }
            ngx_log_error_core(NGX_LOG_ALERT,err,"waitpid() failed!");
            return;
        }
        one = 1;
        if(WTERMSIG(status))
        {
            ngx_log_error_core(NGX_LOG_ALERT,0,"pid = %P exited on signal %d!",pid,WTERMSIG(status)); //获取使子进程终止的信号编号
        }
        else
        {
            ngx_log_error_core(NGX_LOG_NOTICE,0,"pid = %P exited with code %d!",pid,WEXITSTATUS(status)); //WEXITSTATUS()获取子进程传递给exit或者_exit参数的低八位
        }
    }
    return;
}
