#ifndef WLOG_ERROR_H
#define WLOG_ERROR_H

#include <stddef.h>

typedef struct error {
    int code;
    const char *info;
} error_t;

#define error(code, info) (error_t) {code, info}
#define OK (error_t) {0, NULL}

#define ERR_UNKNOWN_COMMAND 1
#define ERR_INPUT 2
#define ERR_LOGIC 3
#define ERR_ARGUMENTS 4

int err_handle(error_t error);
int err_occured(error_t error);

#endif //WLOG_ERROR_H
