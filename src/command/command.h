#ifndef WLOG_COMMAND_H
#define WLOG_COMMAND_H

#include "../util/error.h"

typedef struct command {
    void (*execute)(const char *name);
    char *name;
    char *shortname;
    char *description;
    char *arg_list;
} command_t;

command_t cmd_for_name(char *name, command_t *commands, command_t *default_command);

int cmd_exists(command_t *command);

void cmd_execute(command_t command);

#endif //WLOG_COMMAND_H
