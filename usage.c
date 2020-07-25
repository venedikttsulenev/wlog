#include "usage.h"
#include "interactive.h"
#include "print.h"

static command_t COMMANDS[];

result_t usage_interactive_mode() {
    return imode_run();
}

result_t usage_print_help() {
    print_help(COMMANDS);
    return OK;
}

result_t usage_print_version() {
    print_version();
    return OK;
}

static command_t COMMANDS[] = {
        {usage_interactive_mode, NULL,      NULL,  "Run interactive mode"},
        {usage_print_help,       "help",    "h",   "Print help and quit"},
        {usage_print_version,    "version", "v",   "Print version and quit"},
        {}
};

command_t usage_command(char *name) {
    return cmd_for_name(name, COMMANDS, &COMMANDS[0]);
}