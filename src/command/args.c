#include "args.h"
#include <stdlib.h>
#include <string.h>
#include "../util/str.h"

static const char *wrong_time_format_message = "wrong time format";

void args_time(char *time_str, double *dest) {
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

void args_task(args_t *args) {
    args->task = strtok(NULL, TOKEN_DELIMETERS);
    if (!args->task) {
        err_set(ERR_ARGUMENTS, "expected task argument");
    } else if (strlen(args->task) >= 31) {
        err_set(ERR_ARGUMENTS, "31 characters maximum allowed for task name");
    } else {
        char *c = invalid_task_character(args->task);
        if (c) {
            err_set(ERR_ARGUMENTS, format_str("invalid task name character at position %d", c - args->task + 1));
        }
    }
}

void args_time_and_task(args_t *args) {
    args_task(args);
    if (err_occured()) {
        return;
    }
    char *time_str = strtok(NULL, TOKEN_DELIMETERS);
    if (!time_str) {
        err_set(ERR_ARGUMENTS, "expected two arguments");
    } else {
        args_time(time_str, &args->time_seconds);
    }
}