#ifndef _logger_h_
#define _loger_h_

#ifdef __cplusplus
extern "C" {
#endif



#define LOG_ERROR(...) log_print("ERROR", __FILE__, __LINE__, __VA_ARGS__)

void log_print(const char *tag, const char *file, int line, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif