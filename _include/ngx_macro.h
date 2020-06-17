#ifndef __NGX_MACRO_H__
#define __NGX_MACRO_H__
//宏定义

//显示错误最大长度
#define NGX_MAX_ERROR_STR   2048

#define ngx_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))    

#define NGX_MAX_UINT32_VALUE   (uint32_t) 0xffffffff
#define NGX_INT64_LEN          (sizeof("-9223372036854775808") - 1)  

//日志等级
#define NGX_LOG_STDERR          0   //控制台错误    stderr
#define NGX_LOG_EMERG           1   //紧急          memrg
#define NGX_LOG_ALERT           2   //警戒          alert
#define NGX_LOG_CRIT            3   //严重          crit
#define NGX_LOG_ERR             4   //错误          error
#define NGX_LOG_WARN            5   //警告          warn
#define NGX_LOG_NOTICE          6   //注意          notice
#define NGX_LOG_INFO            7   //信息          info
#define NGX_LOG_DEBUG           8   //调试          debug

//日志输出路径
#define NGX_ERROR_LOG_PATH      "error.log"

#endif