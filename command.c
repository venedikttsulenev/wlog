#include <stdio.h>
#include <string.h>
#include <time.h>
#include "command.h"
#include "error.h"
#include "interactive_mode.h"
#include "print.h"

#define MAX_INPUT_STR_LENGTH 256

result_t interactive_mode_command() {
    static const char *PROMPT = "> ";
    char input_str[MAX_INPUT_STR_LENGTH];
    imode_state_t state;
    init_interactive_mode(&state);
    do {
        fputs(PROMPT, stdout);
        result_t result = OK;
        if (!fgets(input_str, MAX_INPUT_STR_LENGTH, stdin)) {
            result = error(ERR_INPUT, NULL);
        } else {
            unsigned int time_offset = strlen(PROMPT) + strlen(input_str) - 1;
            char *tok = strtok(input_str, IMODE_TOKEN_DELIMETERS);
            imode_command_t *command = get_imode_command_for_str(tok);
            if (command == NULL) {
                result = error(ERR_UNKNOWN_COMMAND, tok);
            } else {
                print_current_time(time_offset);
                result = command->execute(&state);
            }
        }
        handle(result);
    } while (!state.stopped);
    return OK;
}

result_t print_help_command() {
    print_help();
    return OK;
}

result_t print_version_command() {
    print_version();
    return OK;
}

result_t unknown_command(int argc, char **argv) {
    return error(ERR_UNKNOWN_COMMAND, argv[1]);
}

static command_t COMMANDS[] = {
        {interactive_mode_command, NULL,      NULL, "Run in interactive mode"},
        {print_help_command,       "help",    "h",  "Print help and quit"},
        {print_version_command,    "version", "v",  "Print version and quit"},
        {}
};
static const command_t UNKNOWN_COMMAND = {unknown_command};

#define DEFAULT_COMMAND COMMANDS[0]

command_t command_for_str(char *str) {
    if (NULL == str)
        return DEFAULT_COMMAND;
    for (command_t *command = COMMANDS; command->execute != NULL; ++command) {
        if (command->name || command->shortname) {
            if (0 == strcmp(str, command->shortname) || 0 == strcmp(str, command->name))
                return *command;
        }
    }
    return UNKNOWN_COMMAND;
}

command_t *get_commands() {
    return COMMANDS;
}