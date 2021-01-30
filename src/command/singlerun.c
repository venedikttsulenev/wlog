#include "singlerun.h"
#include "interactive.h"
#include "../util/print.h"

static command_t COMMANDS[];

void singlerun_interactive_mode() {
    imode_run();
}

void singlerun_help() {
    print_help(COMMANDS);
}

void singlerun_version() {
    print_version();
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