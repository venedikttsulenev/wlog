#include "args.h"
#include <stdlib.h>
#include <string.h>

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

void args_task(args_t *args) {
    args->task = strtok(NULL, TOKEN_DELIMETERS);
    if (!args->task) {
        err_set(ERR_ARGUMENTS, "expected task argument");
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