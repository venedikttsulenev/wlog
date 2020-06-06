#include <string.h>
#include "command.h"
#include "error.h"

result_t unknown(const char *name) {
    return error(ERR_UNKNOWN_COMMAND, name);
}

#define unknown_command(name) (command_t) {unknown, name};

command_t command_for_name(char *name, command_t *commands, command_t *default_command) {
    if (NULL == name)
        return *default_command;
    for (command_t *command = commands; command->execute != NULL; ++command) {
        if ((command->shortname && (0 == strcmp(name, command->shortname))) ||
            (command->name      && (0 == strcmp(name, command->name)))) {
                return *command;
        }
    }
    return unknown_command(name);
}

int command_exists(command_t *command) {
    return command->execute != NULL;
}

result_t execute_command(command_t command) {
    return command.execute(command.name);
}