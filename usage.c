#include "usage.h"
#include "interactive_mode.h"
#include "print.h"

result_t interactive_mode_command() {
    return run_interactive_mode();
}

result_t print_help_command() {
    print_help();
    return OK;
}

result_t print_version_command() {
    print_version();
    return OK;
}

static command_t COMMANDS[] = {
        {interactive_mode_command, NULL,      NULL, "Run interactive mode"},
        {print_help_command,       "help",    "h",  "Print help and quit"},
        {print_version_command,    "version", "v",  "Print version and quit"},
        {}
};

command_t get_command(char *name) {
    return command_for_name(name, COMMANDS, &COMMANDS[0]);
}

command_t *get_supported_commands() {
    return COMMANDS;
}