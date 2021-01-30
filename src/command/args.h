#include "../util/error.h"

#ifndef WLOG_ARGS_H
#define WLOG_ARGS_H

#define TOKEN_DELIMETERS " \t\n"

typedef struct {
    char *task;
    double time_seconds;
} args_t;

void args_task(args_t *args);

void args_time_and_task(args_t *args);

#endif //WLOG_ARGS_H
