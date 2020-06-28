#include <string.h>
#include "command.h"

result_t unknown(const char *name) {
    return error(ERR_UNKNOWN_COMMAND, name);
}

command_t cmd_for_name(char *name, command_t *commands, command_t *default_command) {
    if (NULL == name)
        return *default_command;
    for (command_t *command = commands; command->execute != NULL; ++command) {
        if ((command->shortname && (0 == strcmp(name, command->shortname))) ||
            (command->name      && (0 == strcmp(name, command->name)))) {
                return *command;
        }
    }
    return (command_t) {unknown, name};
}

int cmd_exists(command_t *command) {
    return command->execute != NULL;
}

result_t cmd_execute(command_t command) {
    return command.execute(command.name);
}