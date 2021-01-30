#ifndef WLOG_ERROR_H
#define WLOG_ERROR_H

#include <stddef.h>

typedef struct error {
    int code;
    const char *info;
} error_t;

#define ERR_UNKNOWN_COMMAND 1
#define ERR_INPUT 2
#define ERR_LOGIC 3
#define ERR_ARGUMENTS 4

void err_set(int code, const char *info);
int err_occured();
int err_handle();

#endif //WLOG_ERROR_H
