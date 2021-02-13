#include "args.h"
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "../util/str.h"

static const char *wrong_time_format_message = "wrong time format";

char *next_arg() {
    return strtok(NULL, TOKEN_DELIMETERS);
}

void parse_time(double *dest, char *time_str) {
    char *endptr;
    long num1 = strtol(time_str, &endptr, 10);
    if (num1 <= 0) {
        err_set(ERR_ARGUMENTS, wrong_time_format_message);
        return;
    }
    if (*endptr == '\0' || (*(endptr + 1) == '\0' && *endptr == 'm')) { // '1m' or just '1'
        *dest = 60 * (double) num1;
    } else if (*endptr == 'h') { // '1h'
        if (*(endptr + 1) == '\0') {
            *dest = 3600 * (double) num1;
        } else {
            long num2 = strtol(endptr + 1, &endptr, 10);
            if (num2 < 0) {
                err_set(ERR_ARGUMENTS, wrong_time_format_message);
                return;
            }
            if (*endptr == '\0' || (*(endptr + 1) == '\0' && *endptr == 'm')) { // '1h30m' or '1h30'
                *dest = (double) (num1 * 3600 + num2 * 60);
            } else {
                err_set(ERR_ARGUMENTS, wrong_time_format_message);
                return;
            }
        }
    } else {
        err_set(ERR_ARGUMENTS, wrong_time_format_message);
    }
}

char *invalid_task_character(char* task) {
    for (char *c = task; *c != '\0'; ++c) {
        if (!strchr("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-/\\:.", *c)) {
            return c;
        }
    }
    return NULL;
}

void parse_task_name(char **dest, char *str) {
    if (strlen(str) >= 31) {
        err_set(ERR_ARGUMENTS, "31 characters maximum allowed for task name");
    } else {
        char *c = invalid_task_character(str);
        if (c) {
            err_set(ERR_ARGUMENTS, format_str("invalid task name character at position %d", c - str + 1));
        }
    }
    *dest = str;
}

const char *argname[] = {
        "time",
        "task",
        "newname"
};

void parse_arg(args_t *args, int argument) {
    char *arg_str = next_arg();
    if (!arg_str) {
        err_set(ERR_ARGUMENTS, format_str("Expected '%s' argument", argname[argument]));
        return;
    }
    switch (argument) {
        case ARG_TIME:
            parse_time(&args->time_seconds, arg_str);
            break;
        case ARG_TASK:
            parse_task_name(&args->task, arg_str);
            break;
        case ARG_NEWNAME:
            parse_task_name(&args->newname, arg_str);
            break;
        default:
            err_set(ERR_INTERNAL, "unknown argument type");
            return;
    }
}

args_t args_get(int count, ...) {
    args_t args = {NULL, NULL, -1};
    va_list ap;
    va_start(ap, count);
    while (count--) {
        parse_arg(&args, va_arg(ap, int));
    }
    va_end(ap);
    return args;
}