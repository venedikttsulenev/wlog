#include "error.h"
#include "print.h"

#define ERR_NONE (error_t) {0, NULL}

static const char *ERR_MSG[] = {
        NULL,
        "Unknown command",
        "Input error",
        "Error",
        "Argument error"
};

static error_t err = ERR_NONE;

void err_set(int code, const char *info) {
    err.code = code;
    err.info = info;
}

int err_occured() {
    return err.code;
}

int err_handle() {
    int code = err.code;
    if (code) {
        print_error(ERR_MSG[code], err.info);
    }
    err = ERR_NONE;
    return err.code;
}
