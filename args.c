#include "args.h"
#include <stdlib.h>
#include <string.h>

static const char *wrong_time_format_message = "wrong time format";

error_t args_time(char *time_str, double *dest) {
    char *endptr;
    long num1 = strtol(time_str, &endptr, 10);
    if (num1 <= 0) {
        return error(ERR_ARGUMENTS, wrong_time_format_message);
    }
    if (*endptr == '\0' || (*(endptr + 1) == '\0' && *endptr == 'm')) { // '1m' or just '1'
        *dest = 60 * (double) num1;
    } else if (*endptr == 'h') { // '1h'
        if (*(endptr + 1) == '\0') {
            *dest = 3600 * (double) num1;
        } else {
            long num2 = strtol(endptr + 1, &endptr, 10);
            if (num2 < 0) {
                return error(ERR_ARGUMENTS, wrong_time_format_message);
            }
            if (*endptr == '\0' || (*(endptr + 1) == '\0' && *endptr == 'm')) { // '1h30m' or '1h30'
                *dest = (double) (num1 * 3600 + num2 * 60);
            } else {
                return error(ERR_ARGUMENTS, wrong_time_format_message);
            }
        }
    } else {
        return error(ERR_ARGUMENTS, wrong_time_format_message);
    }
    return OK;
}

error_t args_task(args_t *args) {
    args->task = strtok(NULL, TOKEN_DELIMETERS);
    if (!args->task) {
        return error(ERR_ARGUMENTS, "expected task argument");
    }
    return OK;
}

error_t args_time_and_task(args_t *args) {
    error_t error = args_task(args);
    if (!err_occured(error)) {
        char *time_str = strtok(NULL, TOKEN_DELIMETERS);
        if (!time_str) {
            error = error(ERR_ARGUMENTS, "expected two arguments");
        } else {
            error = args_time(time_str, &args->time_seconds);
        }
    }
    return error;
}