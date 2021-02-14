#include "../util/error.h"

#ifndef WLOG_ARGS_H
#define WLOG_ARGS_H

#define TOKEN_DELIMETERS " \t\n"

#define ARG_TIME 0
#define ARG_TASK 1
#define ARG_TASK2 2

typedef struct {
    char *task;
    char *task2;
    double time_seconds;
} args_t;

args_t args_get(int count, ...);

#endif //WLOG_ARGS_H
