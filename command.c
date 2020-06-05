#include <string.h>
#include "command.h"
#include "error.h"

result_t unknown_command(int argc, char **argv) {
    return error(ERR_UNKNOWN_COMMAND, argv[1]);
}

#define UNKNOWN_COMMAND (command_t) {unknown_command};

command_t command_for_name(char *name, command_t *commands, command_t *default_command) {
    if (NULL == name)
        return *default_command;
    for (command_t *command = commands; command->execute != NULL; ++command) {
        if ((command->shortname && (0 == strcmp(name, command->shortname))) ||
            (command->name      && (0 == strcmp(name, command->name)))) {
                return *command;
        }
    }
    return UNKNOWN_COMMAND;
}

int command_exists(command_t *command) {
    return command->execute != NULL;
}