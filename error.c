#include "error.h"
#include <stdio.h>

static const char *ERR_MSG[] = {
        NULL,
        "Unknown command",
        "Input error",
        "Error",
        "Argument error"
};

int handle(error_t error) {
    if (error.code) {
        if (error.info) {
            printf("%s: %s\n", ERR_MSG[error.code], error.info);
        } else {
            printf("%s\n", ERR_MSG[error.code]);
        }
    }
    return error.code;
}