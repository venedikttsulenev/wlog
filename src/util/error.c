#include "error.h"
#include "print.h"

static const char *ERR_MSG[] = {
        NULL,
        "Unknown command",
        "Input error",
        "Error",
        "Argument error"
};

int err_handle(error_t error) {
    if (error.code) {
        print_error(ERR_MSG[error.code], error.info);
    }
    return error.code;
}

int err_occured(error_t error) {
    return error.code;
}