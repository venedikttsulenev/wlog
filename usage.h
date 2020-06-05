#ifndef WLOG_USAGE_H
#define WLOG_USAGE_H

#include "command.h"

command_t *get_supported_commands();
command_t get_command(char *name);

#endif //WLOG_USAGE_H