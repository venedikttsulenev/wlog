#ifndef WLOG_COMMAND_H
#define WLOG_COMMAND_H

#include "error.h"

typedef error_t result_t;

typedef struct command {
    result_t (*execute)(const char *name);
    char *name;
    char *shortname;
    char *description;
    char *arg_description;
} command_t;

command_t cmd_for_name(char *name, command_t *commands, command_t *default_command);
int cmd_exists(command_t *command);

result_t cmd_execute(command_t command);

#endif //WLOG_COMMAND_H
