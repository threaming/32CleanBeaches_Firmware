/*
 * debug.h
 *
 *  Created on: 26.03.2019
 *      Author: zajost
 */

#ifndef SOURCES_UTILS_DEBUG_H_
#define SOURCES_UTILS_DEBUG_H_


#include <stdio.h>
#include <stdarg.h>


// https://mcuoneclipse.com/2015/11/21/gnu-gcc-printf-and-builtin-optimizations/
// https://mcuoneclipse.com/2016/08/06/semihosting-again-with-nxp-kinetis-sdk-v2-0/

#define LL_TRACE      0
#define LL_DEBUG      1
#define LL_INFO       2
#define LL_WARN       3
#define LL_ERROR      4
#define LL_FATAL      5


#define Loglevel        LL_DEBUG


extern const char *loglevels[];// = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};


static inline void logif(uint8_t level, int condition, const char *file, const char *func, int line, const char *format, ...)
{
  if (condition)
  {
    va_list args;
    va_start(args, format);
    fprintf(stdout, loglevels[level]);
    fprintf(stdout, "|");

    //fprintf(stdout, file);
    //fprintf(stdout, ":");
    fprintf(stdout, func);
    fprintf(stdout, ":%d|", line);

    vfprintf(stdout, format, args);
    fprintf(stdout, "\r\n");
    va_end(args);
  }
}


#if (Loglevel <= LL_TRACE)
  #define tlogif(condition, ...)      logif(LL_TRACE, condition, __FILE__, __func__, __LINE__, __VA_ARGS__)
  #define tlog(...)                   logif(LL_TRACE, 1, __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
  #define tlogif(condition, ...)
  #define tlog(...)
#endif

#if (Loglevel <= LL_DEBUG)
  #define dlogif(condition, ...)      logif(LL_DEBUG, condition, __FILE__, __func__, __LINE__, __VA_ARGS__)
  #define dlog(...)                   logif(LL_DEBUG, 1, __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
  #define dlogif(condition, ...)
  #define dlog(...)
#endif

#if (Loglevel <= LL_INFO)
  #define ilogif(condition, ...)      logif(LL_INFO, condition, __FILE__, __func__, __LINE__, __VA_ARGS__)
  #define ilog(...)                   logif(LL_INFO, 1, __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
  #define ilogif(condition, ...)
  #define ilog(...)
#endif

#if (Loglevel <= LL_WARN)
  #define wlogif(condition, ...)      logif(LL_WARN, condition, __FILE__, __func__, __LINE__, __VA_ARGS__)
  #define wlog(...)                   logif(LL_WARN, 1, __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
  #define wlogif(condition, ...)
  #define wlog(...)
#endif

#if (Loglevel <= LL_ERROR)
  #define elogif(condition, ...)      logif(LL_ERROR, condition, __FILE__, __func__, __LINE__, __VA_ARGS__)
  #define elog(...)                   logif(LL_ERROR, 1, __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
  #define elogif(condition, ...)
  #define elog(...)
#endif

#if (Loglevel <= LL_FATAL)
  #define flogif(condition, ...)      logif(LL_FATAL, condition, __FILE__, __func__, __LINE__, __VA_ARGS__)
  #define flog(...)                   logif(LL_FATAL, 1, __FILE__, __func__, __LINE__, __VA_ARGS__)
#else
  #define flogif(condition, ...)
  #define flog(...)
#endif


#endif /* SOURCES_UTILS_DEBUG_H_ */
