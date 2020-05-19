#ifndef WLOG_COMMAND_H
#define WLOG_COMMAND_H

#include "error.h"

typedef error_t result_t;

typedef struct {
    result_t (*execute)(int argc, char **argv);
    char *name;
    char *shortname;
    char *description;
} command_t;

command_t command_for_str(char *str);
command_t *get_commands();

#endif //WLOG_COMMAND_H
