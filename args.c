#include "args.h"
#include <stdlib.h>

error_t args_time(char *time_str, double *dest) {
    char *endptr;
    long num1 = strtol(time_str, &endptr, 10);
    if (*endptr == '\0' || (*(endptr + 1) == '\0' && *endptr == 'm')) { // '1m' or just '1'
        *dest = 60 * (double) num1;
    } else if (*endptr == 'h') { // '1h'
        if (*(endptr + 1) == '\0') {
            *dest = 3600 * (double) num1;
        } else {
            long num2 = strtol(endptr + 1, &endptr, 10);
            if (*endptr == '\0' || (*(endptr + 1) == '\0' && *endptr == 'm')) { // '1h30m' or '1h30'
                *dest = (double) (num1 * 3600 + num2 * 60);
            } else {
                return error(ERR_ARGUMENTS, "wrong time format");
            }
        }
    } else {
        return error(ERR_ARGUMENTS, "wrong time format");
    }
    return OK;
}