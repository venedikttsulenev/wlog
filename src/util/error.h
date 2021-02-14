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
#define ERR_INTERNAL 5

void err_set(int code, const char *info);
int err_occured();
void err_print();
int err_code();
void err_reset();

#endif //WLOG_ERROR_H
