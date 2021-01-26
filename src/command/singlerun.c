#include "singlerun.h"
#include "interactive.h"
#include "../util/print.h"

static command_t COMMANDS[];

result_t singlerun_interactive_mode() {
    return imode_run();
}

result_t singlerun_help() {
    print_help(COMMANDS);
    return OK;
}

result_t singlerun_version() {
    print_version();
    return OK;
}

static command_t COMMANDS[] = {
        {singlerun_interactive_mode, NULL, NULL, "Run interactive mode"},
        {singlerun_help,    "help",    "h",      "Print help and quit"},
        {singlerun_version, "version", "v",      "Print version and quit"},
        {}
};

command_t singlerun_command(char *name) {
    return cmd_for_name(name, COMMANDS, &COMMANDS[0]);
}