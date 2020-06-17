#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "ngx_macro.h"
#include "ngx_func.h"
#include "ngx_c_conf.h"

//释放资源
static void freeresource();

pid_t   ngx_pid;
pid_t   ngx_parent;

int main(int argc, char* const *argv)
{
    CConfig *p_config = CConfig::GetInstance();
    if(p_config->Load("nginx.conf") == false)          
    {   
        ngx_log_init();
        ngx_log_stderr(0,"配置文件[%s]载入失败，退出!","nginx.conf");
    }

    fprintf(stderr,"hello world. \n");
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