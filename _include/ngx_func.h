#ifndef __NGX_FUNC_H__
#define __NGX_FUNC_H__

#include <sys/types.h>

//字符串相关
void Rtrim(char *string);
void Ltrim(char *string);   

//日志 打印输出相关
//log.cxx
void   ngx_log_init();
void   ngx_log_stderr(int err, const char *fmt, ...);
void   ngx_log_error_core(int level,  int err, const char *fmt, ...);
u_char *ngx_log_errno(u_char *buf, u_char *last, int err);
//printf.cxx
u_char *ngx_snprintf(u_char *buf, size_t max, const char *fmt, ...);
u_char *ngx_slprintf(u_char *buf, u_char *last, const char *fmt, ...);
u_char *ngx_vslprintf(u_char *buf, u_char *last,const char *fmt,va_list args);

#endif