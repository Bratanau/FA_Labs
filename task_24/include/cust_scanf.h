#ifndef CUST_SCANF_H
#define CUST_SCANF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

typedef enum {
    OVER_SCANF_SUCCESS = 0,
    OVER_SCANF_INVALID_FORMAT,
    OVER_SCANF_INVALID_INPUT,
    OVER_SCANF_BUFFER_OVERFLOW,
    OVER_SCANF_UNKNOWN_FLAG,
    OVER_SCANF_INVALID_BASE,
    OVER_SCANF_END_OF_INPUT
} over_scanf_status_t;

int overfscanf(FILE *stream, const char *format, ...);

int oversscanf(const char *str, const char *format, ...);

#endif
