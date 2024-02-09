/** @file logging.h
 * Logging Library for PowerOS.
 * This library provides macros to use when logging debug, information,
 * warnings, and errors. In development mode, logs are output via uart to the
 * DEBUG pin. In production mode, logs are stored in a buffer.
*/

#ifndef _logging_h_
#define _logging_h_

////////////////////////////////////////
// Lib Config //

#define LOGGING_DEFAULT_LVL     LOG_LVL_DEBUG

#define LOGGING_UART_BAUDRATE   9600

#define LOGGING_DEFAULT_TAG     "log"

////////////////////////////////////////



// Include stdio.h because this lib is pretty much just a wrapper around printf
//
#include <stdio.h>

// Set a LOG_LVL if one is not defined.
//
#ifndef LOG_LVL
#   define LOG_LVL  LOGGING_DEFAULT_LVL
#endif


// Set a default LOG_TAG if not set.
// This means you should #define a tag before including the lib to supress the
// redefinition warning. 
// OR
// You should #undef LOG_TAG before #define'ing LOG_TAG.
// OR
// Put up with that gross, ugly warning somehow.
//
#ifndef LOG_TAG
#   define LOG_TAG  LOGGING_DEFAULT_TAG
#endif


#define LOG_LVL_DISABLE 0
#define LOG_LVL_ERROR   1
#define LOG_LVL_WARNING 2
#define LOG_LVL_INFO    3
#define LOG_LVL_DEBUG   4


#if ((LOG_LVL) && (LOG_LVL >= LOG_LVL_DEBUG))
#   define LOG_DEBUG(f_, ...) printf("DEBUG "LOG_TAG": "f_"\r\n", ##__VA_ARGS__)
#   define LOG_DEBUG_TAG(tag, f_, ...) printf("DEBUG "tag": "f_"\r\n", ##__VA_ARGS__)
#else
#   define LOG_DEBUG(...)
#   define LOG_DEBUG_TAG(...)
#endif

#if ((LOG_LVL) && (LOG_LVL >= LOG_LVL_INFO))
#   define LOG_INFO(f_, ...) printf(" INFO "LOG_TAG": "f_"\r\n", ##__VA_ARGS__)
#   define LOG_INFO_TAG(tag, f_, ...) printf("INFO "tag": "f_"\r\n", ##__VA_ARGS__)
#else
#   define LOG_INFO(...)
#   define LOG_INFO_TAG(...)
#endif

#if ((LOG_LVL) && (LOG_LVL >= LOG_LVL_WARNING))
#   define LOG_WARNING(f_, ...) printf(" WARN "LOG_TAG": "f_"\r\n", ##__VA_ARGS__)
#   define LOG_WARNING_TAG(tag, f_, ...) printf("WARN "tag": "f_"\r\n", ##__VA_ARGS__)
#else
#   define LOG_WARNING(...)
#   define LOG_WARNING_TAG(...)
#endif

#if ((LOG_LVL) && (LOG_LVL >= LOG_LVL_ERROR))
#   define LOG_ERROR(f_, ...) printf("ERROR "LOG_TAG": "f_"\r\n", ##__VA_ARGS__)
#   define LOG_ERROR_TAG(tag, f_, ...) printf("ERROR "tag": "f_"\r\n", ##__VA_ARGS__)
#else
#   define LOG_ERROR(...)
#   define LOG_ERROR_TAG(...)
#endif

void    logging_init (void);
void    putch (char data);

/**
 * Check if transmit buffer is empty.
 * 
 * @returns     1 if buffer is empty, 0 otherwise.
*/
unsigned char
logging_buffer_empty (void);

#endif

// EOF //
