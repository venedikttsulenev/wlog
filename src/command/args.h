#include "../util/error.h"

#ifndef WLOG_ARGS_H
#define WLOG_ARGS_H

#define TOKEN_DELIMETERS " \t\n"

typedef struct {
    const char *task;
    double time_seconds;
    error_t error;
} args_t;

error_t args_task(args_t *args);

error_t args_time_and_task(args_t *args);

#endif //WLOG_ARGS_H
